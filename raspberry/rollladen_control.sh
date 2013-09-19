#!/bin/bash

echo "Loading i2c driver"
gpio load i2c 10 | exit

echo "Polling starts."

while :
do
	INT="$(gpio read 7)"
	if [ "$INT" -eq "0" ];
	then
		echo "INT"
		
		SW="$(i2cget -y 1 0x22 0x31)"
		if [ "$?" == "0" ]
		then
			echo "Read value $SW."
			
			if [ "$SW" == "0x01" ]
			then
			  echo "Going up"
			  i2cset -y 1 0x21 0x23
			elif [ "$SW" == "0x02" ]
			then
			  echo "Going down"
			  i2cset -y 1 0x21 0x33 
			elif [ "$SW" == "0x03" ]
			then
			  echo "Stop"
			  i2cset -y 1 0x21 0x13 
			else
			  echo "Unknown answer value: $SW"
			  $(exit 1)
			fi
			
			echo "ERR: $?"
			if [ "$?" == "0" ]; then
			  echo "Reset."
			  i2cset -y 1 0x22 0x40
			fi
		fi
	fi
	
	#sleep 1s;
done

echo "Exit."
