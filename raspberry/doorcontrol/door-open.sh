#!/bin/bash

caller=$(ps -ocommand= -p $PPID)
logger -t shuttercontrol Opening door from shell script via $caller.

gpio load i2c

ret=""
while [[ "$ret" != "0x01" ]]; do
	ret=$(/usr/sbin/i2cget -y 1 0x24 0x90)
	echo $ret
done

