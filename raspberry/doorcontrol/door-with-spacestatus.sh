#!/bin/bash

# Lock the door if SpaceTime is not active
# 30 seconds after it is unlocked

function wait_for_status {
	st=$1
	s=""
	
	while [ "$s" != "$st" ]; do
		s=$(./door-status.sh)
		echo $s
		sleep 1s
	done
}

function jsonval {
	temp=$(echo $json | sed -e 's/\\\\\//\//g' -e 's/[{}]//g' \
	                  | awk -v k="text" '{n=split($0,a,","); for (i=1; i<=n; i++) print a[i]}' \
	                  | sed -e 's/\"\:\"/\|/g' -e 's/[\,]/ /g' -e 's/\"//g' \
	                  | grep -w $prop)
	echo ${temp##*|}
}

function space_is_open {
	json=$(curl -s http://spaceapi.n39.eu/json)
	prop='open'
	
	st=$(jsonval)
	isopen=$(echo "$st" | sed -e 's/.*open:\(.*\)/\1/')
	
	echo "$isopen"
}

TIMEOUT=30
DELAY=5

logger -t lockfailsafe SpaceTime observation restarted with timeout ${TIMEOUT}s and a delay of ${DELAY}s.

while [[ true ]]; do
	# check the door status
	s=$(./door-status.sh)
	echo "Door state: $s"
	
	# if unlocked
	if [ "$s" == "0x0c" ]; then
		echo "Door is unlocked."
		logger -t lockfailsafe SpaceTime observation engaged.
	
		# check space status
		isopen=''
		timeout=$TIMEOUT
		
		while [ "$timeout" -gt "0" ]; do
			echo -n "Check for Space Status == Open: "
			isopen=$(space_is_open)			
			echo "$isopen"			
			
			
			# if closed, decement timeout
			if [ "$isopen" == "true" ]; then
				if [ "$timeout" -lt "$TIMEOUT" ]; then
					# blink off
					i2cset -y 1 0x22 0xa0
				fi

				timeout=$TIMEOUT
				echo "SpaceTime active, timeout reset."
			else
				echo -n "No active SpaceTime detected. "
				echo "$timeout seconds remaining until door is locked."
				let "timeout=$timeout-$DELAY"
				# slow blink and beep
				i2cset -y 1 0x22 0x21
				i2cset -y 1 0x22 0x12
			fi
			
			sleep $DELAY
		done; # while timeout > 0				
		
		# now it is closed -> close the door
		echo "Closing door since SpaceStatus is closed!"
		logger -t lockfailsafe Closing door due to inactive SpaceTime status.
		# with fast blink and beep-beep
		i2cset -y 1 0x22 0x22
		i2cset -y 1 0x22 0x9a
		./door-close.sh
		
		# give the door some time to close
		sleep 5
	
		# blink off
		i2cset -y 1 0x22 0xa0	
	fi # unlocked

	sleep 2
done
