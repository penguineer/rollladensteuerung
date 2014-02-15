#!/bin/bash

CALLERID=$1
PIN=$2

WHITELIST=$3

if grep -Fxq "$CALLERID $PIN" $WHITELIST; then
  echo -n OK
else
  echo -n FAIL
fi
