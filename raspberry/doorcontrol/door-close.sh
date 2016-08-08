#!/bin/bash


ret=""
while [[ "$ret" != "0x01" ]]; do
        ret=$(/usr/sbin/i2cget -y 1 0x23 0xa0)
        echo $ret
done
