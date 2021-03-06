#!/bin/bash

num_users=3
num_changed_pos=25
size=65536

if [ ! -fbuild/keycreator ]
then
    echo "file `build/keycreator is not exists`"
    echo "compile before using"
    exit 1
fi

if [ ! -fbuild/cipher ]
then
    echo "file `build/cipher is not exists`"
    echo "compile before using"
    exit 1
fi

if [ ! -fbuild/diffpositions ]
then
    echo "file `build/diffpositions is not exists`"
    echo "compile before using"
    exit 1
fi

while getopts u:s:d: opt; do
  case $opt in
  u)
      num_users=$OPTARG
      ;;
  s)
      size=$OPTARG
      ;;
  d)
      num_changed_pos=$OPTARG
      ;;
  esac
done


for (( i=0; i<num_users; i++ ))
do
	for (( j=0; j<num_changed_pos; j++ ))
	do
		idx=$(( $i * $num_changed_pos + $j ))
		positions[idx]=$((RANDOM%(size*8)))
	done
done


echo "<?xml version="1.0"?>" > config.xml

echo "    <key id='enc'>" >> config.xml
echo "    </key>" >> config.xml


for (( i=0; i<num_users; i++ ))
do
	echo "    <key id='$i'>" >> config.xml
	for (( j=0; j<num_changed_pos; j++ ))
	do
		idx=$(( $i * $num_changed_pos + $j ))
		echo "        <change position='${positions[$idx]}'/> " >> config.xml
	done
	echo "    </key>" >> config.xml
done

dd bs=16 count=1 skip=0 if=/dev/urandom of=aes_key > /dev/null 2> /dev/null
build/keycreator --aes_key aes_key --decr_keys_config config.xml -o keys -s $size
rm aes_key

dd bs=$size count=1 skip=0 if=/dev/urandom of=origin > /dev/null 2> /dev/null

build/cipher -i origin -o enc -k keys/enc -e

for (( i=0; i<num_users; i++ ))
do
	build/cipher -i enc -o dec$i -k keys/$i -d

	differencies=`build/diffpositions origin dec$i`
	diff_counter=0;
	for pos in $differencies; do
		exists=0
		for (( j=0; j<num_changed_pos; j++ ))
		do
			idx=$(( $i * $num_changed_pos + $j ))
			if [ "$pos" -eq "${positions[idx]}" ]
			then 
				exists=1
				diff_counter=$(($diff_counter + 1))
			fi
		done

	if [ "$exists" -ne "1" ]
	then 
		echo "FAILED!"
		exit 2
	fi
	done

	if [ "$diff_counter" -ne "$num_changed_pos" ]
	then 
		echo "FAILED!"
		exit 2
	fi
done


for (( i=0; i<num_users; i++ ))
do
	rm dec$i
done

rm origin
rm -rf keys
rm config.xml
rm enc

echo "test passed FINE"
