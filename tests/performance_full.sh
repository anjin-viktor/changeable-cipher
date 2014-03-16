#!/bin/bash

size=4096
step=4096


for (( ; $size<=65536; size=$size + $step ))
do
	TIMEFORMAT="$size: %lR"
	time ./performance.sh -s $size > /dev/null 2> /dev/null
done