#!/bin/bash

size=4096
step=4096

TIMEFORMAT='%lR'

for (( ; $size<=65536; size=$size + $step ))
do
	time ./performance.sh -s $size > /dev/null 2> /dev/null
done