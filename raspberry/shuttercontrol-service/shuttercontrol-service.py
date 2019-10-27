#!/usr/bin/python3

import signal
import sys
import time

import argparse

import syslog

import paho.mqtt.client as mqtt
import smbus

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


class I2cObserver:
    def __init__(self, address, cb):
        self.address = address
        self.cb = cb

        self.run = True

        self.bus = smbus.SMBus(1)

    def stop(self):
        self.run = False

    def loop(self):
        old_state = dict()
        while self.run:
            # get the state
            state = self._get_state()

            if state is None:
                time.sleep(0.5)
                continue

            new_state = dict()
            # find changes in state
            for k in state.keys():
                if k not in old_state or state[k] != old_state[k]:
                    new_state[k] = state[k]

            if new_state and self.cb is not None:
                self.cb(state, new_state)

            old_state = state

            time.sleep(1)

    def _get_state(self):
        data = self._receive()

        if data is None:
            return None

        state = dict()
        state['green_active'] = bool(data & 0x20)
        state['red_active'] = bool(data & 0x10)
        state['door_closed'] = bool(data & 0x08)
        state['lock_open'] = bool(data & 0x04)
        state['force_close'] = bool(data & 0x02)
        state['force_open'] = bool(data & 0x01)

        return state

    def _receive(self):
        hops = 10
        while hops:
            try:
                d = self.bus.read_word_data(self.address, 0x30)
                data = [d & 0xff, (d & 0xff00) >> 8]
                # data is valid if first byte is binary inversion of second byte
                # and result is not zero
                if data[0] == (data[1] ^ 0xff) and data[0] != 0:
                    return data[0]
            except OSError:
                pass
                # syslog.syslog(syslog.LOG_WARNING, "OS error on I2C receive {}".format(str(e)))
            hops = hops-1
            time.sleep(0.5)

        return None


class BeepHandler:
    def __init__(self, device, mqttclient, topic_base):
        self.device = device
        self.mqttclient = mqttclient
        self.topic_base = topic_base

        self.run = True

        self.bus = smbus.SMBus(1)

        beep_topic = "{0}/{1}".format(self.topic_base, 'Beep')
        mqtt_add_topic_callback(self.mqttclient, beep_topic, self.beep_callback)

    def beep_callback(self, _client, _userdata, message):
        payload = message.payload.decode("utf-8")
        try:
            pattern = int(payload)
            self._beep(pattern)
        except ValueError:
            pass

    def _beep(self, pattern):
        cmd = 0x10 + (pattern % 16)
        cmd += (cmd % 2) * 0x80 # parity bit
        self._i2c_send(cmd)

    def _i2c_send(self, data):
        hops = 10
        while hops:
            try:
                res = self.bus.read_byte_data(self.device, data)
                if res == 0x01:
                    break
            except OSError as e:
                syslog.syslog(syslog.LOG_WARNING, "OS error on I2C receive {}".format(str(e)))
            hops = hops-1
            time.sleep(0.5)


def main():
    parser = argparse.ArgumentParser(
        description="Gatekeeper Door Service")
    parser.add_argument("--mqtthost", help="MQTT host", default="localhost")
    parser.add_argument("--mqttport", help="MQTT port", default=1883)
    parser.add_argument("--topic", help="MQTT topic prefix", default="Netz39/Things/Shuttercontrol")
    parser.add_argument("--i2c", help="I2C device address for the door controller", default=0x22)
    args = parser.parse_args()

    syslog.openlog("shuttercontrol", syslog.LOG_CONS | syslog.LOG_PID, syslog.LOG_USER)
    syslog.syslog(syslog.LOG_INFO, "Starting shuttercontrol service.")

    mqttclient = mqtt.Client()
    mqttclient.on_connect = on_mqtt_connect
    mqttclient.connect(args.mqtthost, args.mqttport, 60)
    mqttclient.loop_start()

    bh = BeepHandler(args.i2c, mqttclient, args.topic)
    while True:
        time.sleep(1)

    mqttclient.loop_stop()

    syslog.syslog(syslog.LOG_INFO, "Shuttercontrol service finished.")
    syslog.closelog()


if __name__ == "__main__":
    signal.signal(signal.SIGINT, sigint_handler)

    main()
