#!/bin/bash

gpio load i2c

i2cget -y 1 0x24 0x30
