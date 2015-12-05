#!/bin/bash
#
# Read the syslog and wait for Asterisk abuse messages.
# Write the IPs into a firewall rule.
#
# Author: tux@netz39.de

LOGFILE=/var/log/syslog 

TAG="asterisk-abuse"

# Check patterns, extend if you find a new pattern
ARE1="asterisk.*Call from.*\\((.*):.*\\).*rejected because extension not found in context"

IP=""

# watch new lines in logfile
tail -fn0 $LOGFILE | while read line ; do
        # Simplified form.... just an if clause for each available pattern
        if [[ $line =~ $ARE1 ]]
        then
        	IP=${BASH_REMATCH[1]}
        	echo $line
        	echo Culprit IP: $IP 
        fi
        
        
        # If there is something in the IP variable
        # try to block it.
        if [ -n "$IP" ]
        then
        	echo Blocking access for IP $IP
        	UFWRES=$(ufw deny from $IP to any)
        	echo $UFWRES
        	
        	logger -t $TAG "Adding block for IP $IP to ufw firewall. $UFWRES"
        	
        	# delete the IP content
        	IP=""
        fi
done # watch logfile

# End of file
