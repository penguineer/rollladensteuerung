#!/bin/bash

gpio mode 4 out
gpio write 4 1

sleep 2

#gpio load i2c
./shuttercontrol

gpio write 4 0
gpio mode 4 tri