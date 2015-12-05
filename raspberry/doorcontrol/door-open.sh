#!/bin/bash

caller=$(ps -ocommand= -p $PPID)
logger -t shuttercontrol "Opening door from shell script via $caller."

gpio load i2c

ret=""
errcount=0
while [[ "$ret" != "0x01" ]]; do
	ret=$(/usr/sbin/i2cget -y 1 0x23 0x90)
	echo $ret
	errcount=$(($errcount+1))
	if [[ $errcount -eq 10 ]]; then
		logger -t shuttercontrol "I2C seems to be stuck, restarting the hardware."
		sleep 5
		~/unstuck.sh
		sleep 5
	fi
done

