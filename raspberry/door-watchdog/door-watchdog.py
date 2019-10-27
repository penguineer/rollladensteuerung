#!/usr/bin/python3

from enum import Enum

import signal
import sys
import time
import threading
import sched

import argparse

import syslog

import paho.mqtt.client as mqtt

MQTT_MSG_DOOROPEN = "door open"
MQTT_MSG_LOCKOPEN = "door unlocked"
MQTT_MSG_LOCKCLOSE = "door locked"
MQTT_MSG_NONE = "none"

MQTT_CMD_OPEN = "door open"
MQTT_CMD_CLOSE = "door close"


MQTT_TOPICS = {}


def mqtt_add_topic_callback(mqttclient, topic, cb):
    MQTT_TOPICS[topic] = cb

    mqttclient.subscribe(topic)
    mqttclient.message_callback_add(topic, cb)


def on_mqtt_connect(client, _userdata, _flags, rc):
    syslog.syslog(syslog.LOG_INFO, "MQTT connected with code {}".format(rc))
    for topic, cb in MQTT_TOPICS.items():
        client.subscribe(topic)
        client.message_callback_add(topic, cb)


def sigint_handler(_signal, _frame):
    global RUN
    if RUN:
        syslog.syslog(syslog.LOG_INFO, "SIGINT received, breaking run loop.")
        RUN = False
    else:
        syslog.syslog(syslog.LOG_INFO, "SIGINT received after breaking run loop, exiting.")
        sys.exit(0)


class MqttObserver:
    """
    Generic MQTT observer for one topic.

    Messages on the topic trigger the callback function: cb(topic, message)
    """
    def __init__(self, mqttclient, topic_base, topic_sub, cb):
        if mqttclient is None:
            raise ValueError("MQTT client parameter must not be None")
        self.mqttclient = mqttclient

        self.topic_base = topic_base
        self.topic_sub = topic_sub

        if cb is None:
            raise ValueError("Callback must be provided!")
        self.cb = cb

        mqtt_add_topic_callback(self.mqttclient,
                                self._render_topic(self.topic_base, self.topic_sub),
                                self._mqtt_callback)

    @staticmethod
    def _render_topic(base, sub):
        if sub is None or not len(sub):
            raise ValueError("Sub-topic must be provided!")

        return "{}{}{}".format(base,
                               "/" if base is not None and len(base) else "",
                               sub)

    def _mqtt_callback(self, _client, _userdata, message):
        payload = message.payload.decode("utf-8")

        self.cb(message.topic, payload)


class SpaceStatusObserver:
    def __init__(self, mqttclient, topic_base, status_cb):
        self.mqttclient = mqttclient
        self.topic_base = topic_base

        if status_cb is None:
            raise ValueError("Status Callback must be provided!")
        self.status_cb = status_cb

        self.obs = MqttObserver(self.mqttclient,
                                self.topic_base,
                                "isOpen",
                                self._status_changed)

    def _status_changed(self, _topic, payload):
        is_open = payload == "true"

        syslog.syslog(syslog.LOG_INFO,
                      "Observed space status set to {}open.".format(
                          "" if is_open else "NOT "
                      ))

        self.status_cb(is_open)


class LockObserver:
    def __init__(self, mqttclient, topic_base, lock_cb):
        self.mqttclient = mqttclient
        self.topic_base = topic_base

        # This observer is event based and reacts on the door lock and the open state
        # (open door means it must be unlocked)
        self.locked = None

        if lock_cb is None:
            raise ValueError("Lock Callback must be provided!")
        self.lock_cb = lock_cb

        self.obs = MqttObserver(self.mqttclient,
                                self.topic_base,
                                "Events",
                                self._status_changed)

    def _status_changed(self, _topic, payload):
        open_state = ((payload == MQTT_MSG_DOOROPEN) or (payload == MQTT_MSG_LOCKOPEN))
        close_state = payload == MQTT_MSG_LOCKCLOSE

        assert (not (open_state and close_state)), "Observed open and close state in one message, that's impossible!"

        # exit if we did not observe a relevant state
        if not open_state and not close_state:
            return

        new_locked = None

        if open_state and (self.locked is None or self.locked):
            syslog.syslog(syslog.LOG_INFO, "Door observed as unlocked ({})".format(payload))
            new_locked = False

        if close_state and (self.locked is None or not self.locked):
            syslog.syslog(syslog.LOG_INFO, "Door observed as locked ({})".format(payload))
            new_locked = True

        # if something changed
        if new_locked is not None and new_locked != self.locked:
            self.locked = new_locked
            self.lock_cb(self.locked)


class LockActor:
    """Act on the lock, i.e. lock or unlock."""
    def __init__(self, mqttclient, topic_base):
        self.mqttclient = mqttclient
        self.topic_base = topic_base

    @staticmethod
    def _render_topic(base, sub):
        if sub is None or not len(sub):
            raise ValueError("Sub-topic must be provided!")

        return "{}{}{}".format(base,
                               "/" if base is not None and len(base) else "",
                               sub)

    def lock_door(self):
        self._send_command(MQTT_CMD_CLOSE)

    def _send_command(self, cmd):
        topic = LockActor._render_topic(self.topic_base, 'Command')

        self.mqttclient.publish(topic, cmd, qos=2, retain=False)


class WatchDog:
    """
    States:
        BOOT - Find out about Space and Lock state
        OPEN - Space is open
        COUNTDOWN - In countdown for closing
        CLOSE - Locking the space
        LOCKED - Space is locked
    """
    class WDStates(Enum):
        BOOT = 1
        OPEN = 2
        COUNTDOWN = 3
        LOCKED = 4

    def __init__(self, mqttclient, status_topic_base, door_topic_base, scheduler):
        self.mqttclient = mqttclient
        self.status_topic_base = status_topic_base
        self.door_topic_base = door_topic_base
        self.countdown_duration_s = 30

        # start with the BOOT state
        self.current_state = self.WDStates.BOOT

        # these are created here to get to correct trampolines
        self.state_handlers = {
            self.WDStates.BOOT: self._step_boot,
            self.WDStates.OPEN: self._step_open,
            self.WDStates.COUNTDOWN: self._step_countdown,
            self.WDStates.LOCKED: self._step_locked
        }

        self.countdown_target = None
        self.nextlock_target = None

        self.locked = False
        self.is_open = None

        self.status_obs = SpaceStatusObserver(self.mqttclient, self.status_topic_base, self._status_callback)
        self.lock_obs = LockObserver(self.mqttclient, self.door_topic_base, self._lock_callback)
        self.lock_ac = LockActor(self.mqttclient, self.door_topic_base)

        # get a re-entrant lock for the Watchdog
        self.wd_lock = threading.RLock()

        # beep scheduler and events
        self.beep_sched = scheduler
        self.beep_events = list()
        self.beep_delay_s = 5

    def _status_callback(self, status):
        with self.wd_lock:
            self.is_open = status

    def _lock_callback(self, locked):
        with self.wd_lock:
            self.locked = locked

    def step(self):
        """Do the next step in the state machine, if applicable"""
        with self.wd_lock:
            if self.current_state in self.state_handlers.keys():
                handler = self.state_handlers.get(self.current_state)
                if handler is not None:
                    handler()

    def _step_boot(self):
        with self.wd_lock:
            assert (self.current_state == self.WDStates.BOOT), "Called BOOT handler when not in BOOT state!"

            # check if we got info about state and lock
            if (self.locked is not None) and (self.is_open is not None):
                # check what the next step should be
                if self.is_open:
                    self.current_state = self.WDStates.OPEN
                elif self.locked:
                    self.current_state = self.WDStates.LOCKED
                else:
                    self.current_state = self.WDStates.COUNTDOWN

                syslog.syslog(syslog.LOG_INFO,
                              "Boot-up information acquired, starting normal operation with status {}."
                              .format(self.current_state))

    def _step_open(self):
        with self.wd_lock:
            assert (self.current_state == self.WDStates.OPEN), "Called OPEN handler when not in OPEN state!"

            # go into countdown when state is closed, but the door is open
            if not self.is_open and not self.locked:
                self.current_state = self.WDStates.COUNTDOWN
                syslog.syslog(syslog.LOG_INFO, "Space state is closed while in door is open, going to COUNTDOWN.")

            # not open, but door is already locked, go to closed
            if not self.is_open and self.locked:
                self.current_state = self.WDStates.LOCKED
                syslog.syslog(syslog.LOG_INFO, "Space state is closed and door is locked, going to LOCKED.")

    def _step_countdown(self):
        with self.wd_lock:
            assert (self.current_state == self.WDStates.COUNTDOWN), \
                "Called COUNTDOWN handler when not in COUNTDOWN state!"

            # abort condition 1:
            # space is open again
            if self.is_open:
                self.current_state = self.WDStates.OPEN
                syslog.syslog(syslog.LOG_INFO, "Space has been opened during countdown, going to OPEN.")

            # abort condition 2:
            # door has been locked
            if not self.is_open and self.locked:
                self.current_state = self.WDStates.LOCKED
                syslog.syslog(syslog.LOG_INFO, "Door has been locked, going to LOCKED.")

            # if not in countdown state anymore, reset and return
            if self.current_state != self.WDStates.COUNTDOWN:
                # reset countdown
                self.countdown_target = None
                self._clear_beeps()
                return

            # handle the countdown
            t = int(time.time())
            if self.countdown_target is None:
                # setup
                self.countdown_target = t + self.countdown_duration_s
                self._schedule_beep_train(self.countdown_duration_s)

            elif (t > self.countdown_target) and not self.locked:
                syslog.syslog(syslog.LOG_INFO,
                              "Issuing door lock command, countdown continues until door is locked "
                              "or Space status is open again.")

                # try to lock the door
                self.lock_ac.lock_door()

                # set new countdown, so locking will be re-tried
                self.countdown_target = t + 10
                self._schedule_beep_train(10)

                # when successful, abort condition 2 will apply

    def _step_locked(self):
        with self.wd_lock:
            assert (self.current_state == self.WDStates.LOCKED), "Called LOCKED handler when not in LOCKED state!"

            # go to OPEN, when the space status is open
            if self.is_open:
                self.current_state = self.WDStates.OPEN
                syslog.syslog(syslog.LOG_INFO, "Space status is open, going to OPEN.")

            # go to countdown, when space status is closed, but door has been opened
            if not self.is_open and not self.locked:
                self.current_state = self.WDStates.COUNTDOWN
                syslog.syslog(syslog.LOG_INFO, "Space state is closed while in door is open, going to COUNTDOWN.")

    def _clear_beeps(self):
        with self.wd_lock:
            for evt in self.beep_events:
                try:
                    self.beep_sched.cancel(evt)
                except ValueError:
                    # ignore: event has been removed before
                    pass
            self.beep_events.clear()

    def _schedule_beep_train(self, duration):
        with self.wd_lock:
            for delay in range(0, duration, self.beep_delay_s):
                self._schedule_beep(delay)

    def _schedule_beep(self, delay):
        with self.wd_lock:
            evt = self.beep_sched.enter(delay, 1, self._do_beep)
            self.beep_events.append(evt)

    def _do_beep(self):
        if self.current_state == self.WDStates.COUNTDOWN:
            pass


def main():
    parser = argparse.ArgumentParser(
        description="Door Watchdog")
    parser.add_argument("--mqtthost", help="MQTT host", default="localhost")
    parser.add_argument("--mqttport", help="MQTT port", default=1883)
    parser.add_argument("--topicdoor", help="MQTT door topic prefix", default="Netz39/Things/Door")
    parser.add_argument("--topicstate", help="MQTT state topic prefix", default="Netz39/SpaceAPI")
    args = parser.parse_args()

    syslog.openlog("doorwatchdog", syslog.LOG_CONS | syslog.LOG_PID, syslog.LOG_USER)
    syslog.syslog(syslog.LOG_INFO, "Starting door watchdog.")

    mqttclient = mqtt.Client()
    mqttclient.on_connect = on_mqtt_connect
    mqttclient.connect(args.mqtthost, args.mqttport, 60)
    mqttclient.loop_start()

    # global scheduler
    scheduler = sched.scheduler(time.time, time.sleep)

    # add some code here
    watchdog = WatchDog(mqttclient, args.topicstate, args.topicdoor, scheduler)

    global RUN
    RUN = True

    # this can become an async timer loop issued by the watchdog
    # when Python 3.7 is available
    while RUN:
        watchdog.step()
        scheduler.run(False)
        time.sleep(0.5)

    mqttclient.loop_stop()

    syslog.syslog(syslog.LOG_INFO, "Door watchdog finished.")
    syslog.closelog()


if __name__ == "__main__":
    signal.signal(signal.SIGINT, sigint_handler)

    main()
