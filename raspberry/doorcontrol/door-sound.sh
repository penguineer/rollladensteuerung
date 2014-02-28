#!/bin/bash

amixer set PCM 100%
#echo "$$" > /run/tyr.pid

function wait_for_status {
	st=$1
	s=""
	
	while [ "$s" != "$st" ]; do
		s=$(./door-status.sh)
		echo $s
		sleep 1s
	done
}

while [[ true ]]; do
	#wait for open door
<<<<<<< HEAD
	wait_for_status "0x04"
=======
	wait_for_status "0004"
>>>>>>> 5a3407d... door sound script
        echo "open"

        mpg123 /home/pi/tyr/sounds/`ls /home/pi/tyr/sounds | shuf -n1` &

	#wait for closed door
<<<<<<< HEAD
	wait_for_status "0x0c"
=======
	wait_for_status "000c"
>>>>>>> 5a3407d... door sound script
        echo "closed"
        sleep 2
done
