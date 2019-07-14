#!/usr/bin/python3

import signal
import sys

import argparse

import paho.mqtt.client as mqtt

MQTT_TOPICS = {}


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


def main():
    parser = argparse.ArgumentParser(
        description="Gatekeeper Door Service")
    parser.add_argument("--mqtthost", help="MQTT host", default="localhost")
    parser.add_argument("--mqttport", help="MQTT port", default=1883)
    parser.add_argument("--topic", help="MQTT topic prefix", default="Things/Door")
    args = parser.parse_args()

    mqttclient = mqtt.Client()
    mqttclient.on_connect = on_mqtt_connect
    mqttclient.connect(args.mqtthost, args.mqttport, 60)
    mqttclient.loop_start()

    mqttclient.loop_stop()


if __name__ == "__main__":
    signal.signal(signal.SIGINT, sigint_handler)

    main()
