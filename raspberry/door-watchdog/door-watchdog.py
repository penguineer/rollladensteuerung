#!/usr/bin/python3

import signal
import sys
import time

import argparse

import syslog

import paho.mqtt.client as mqtt

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
    syslog.syslog(syslog.LOG_INFO, "SIGINT received. Exit.")
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

    # add some code here

    mqttclient.loop_stop()

    syslog.syslog(syslog.LOG_INFO, "Door watchdog finished.")
    syslog.closelog()


if __name__ == "__main__":
    signal.signal(signal.SIGINT, sigint_handler)

    main()
