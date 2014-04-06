#!/bin/bash

size=65536
step=65536


for (( ; $size<=8388608; size=$size + $step ))
do
	TIMEFORMAT="$size: %lR"
	time tests/performance.sh -s $size > /dev/null 2> /dev/null
done