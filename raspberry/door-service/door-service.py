#!/usr/bin/python3

import signal
import sys
import time

import argparse

import paho.mqtt.client as mqtt
import smbus

MQTT_TOPICS = {}

MQTT_MSG_DOOROPEN = "door open"
MQTT_MSG_DOORCLOSE = "door closed"
MQTT_MSG_LOCKOPEN = "door unlocked"
MQTT_MSG_LOCKCLOSE = "door locked"
MQTT_MSG_BTNGREEN = "button green"
MQTT_MSG_BTNRED = "button red"
MQTT_MSG_NONE = "none"


def mqtt_add_topic_callback(mqttclient, topic, cb):
    MQTT_TOPICS[topic] = cb

    mqttclient.subscribe(topic)
    mqttclient.message_callback_add(topic, cb)


def on_mqtt_connect(client, _userdata, _flags, rc):
    print("MQTT connected with code %s" % rc)
    for topic, cb in MQTT_TOPICS.items():
        client.subscribe(topic)
        client.message_callback_add(topic, cb)


def sigint_handler(_signal, _frame):
    print("SIGINT received. Exit.")
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
                if data[0] == (data[1] ^ 0xff):
                    return data[0]
            except OSError as e:
                print("OS error on I2C receive {}".format(str(e)))
            hops = hops-1

        return None


class MqttAnnouncer:
    def __init__(self, mqttclient, topic_base):
        self.mqttclient = mqttclient
        self.topic_base = topic_base

    def callback(self, _state, new_state):
        for k in new_state.keys():
            if k == 'green_active' and new_state[k]:
                self._mqtt_send('Button/Events', MQTT_MSG_BTNGREEN)
            if k == 'red_active' and new_state[k]:
                self._mqtt_send('Button/Events', MQTT_MSG_BTNRED)

            if k == 'door_closed':
                self._mqtt_send('Events', MQTT_MSG_DOORCLOSE if new_state[k] else MQTT_MSG_DOOROPEN)
            if k == 'lock_open':
                self._mqtt_send('Events', MQTT_MSG_LOCKOPEN if new_state[k] else MQTT_MSG_LOCKCLOSE)

        print(new_state)

    def _mqtt_send(self, topic_apx, msg):
        topic = "{0}/{1}".format(self.topic_base, topic_apx)
        self.mqttclient.publish(topic, msg, qos=2, retain=False)


def main():
    parser = argparse.ArgumentParser(
        description="Gatekeeper Door Service")
    parser.add_argument("--mqtthost", help="MQTT host", default="localhost")
    parser.add_argument("--mqttport", help="MQTT port", default=1883)
    parser.add_argument("--topic", help="MQTT topic prefix", default="Netz39/Things/Door")
    parser.add_argument("--i2c", help="I2C device address for the door controller", default=0x23)
    args = parser.parse_args()

    mqttclient = mqtt.Client()
    mqttclient.on_connect = on_mqtt_connect
    mqttclient.connect(args.mqtthost, args.mqttport, 60)
    mqttclient.loop_start()

    mqtta = MqttAnnouncer(mqttclient, args.topic)

    obs = I2cObserver(args.i2c, mqtta.callback)
    obs.loop()

    mqttclient.loop_stop()


if __name__ == "__main__":
    signal.signal(signal.SIGINT, sigint_handler)

    main()
