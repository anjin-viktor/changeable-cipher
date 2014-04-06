#!/bin/bash

num_users=3
num_changed_pos=25
size=8388608

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


shift $((OPTIND - 1))



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
		echo "        <change position='${positions[$idx]}' value='1' /> " >> config.xml
	done
	echo "    </key>" >> config.xml
done

dd bs=16 count=1 skip=0 if=/dev/urandom of=aes_key > /dev/null 2> /dev/null

echo build/keycreator --aes_key aes_key --decr_keys_config config.xml -o keys -s $size
echo "Key creation: "
time build/keycreator --aes_key aes_key --decr_keys_config config.xml -o keys -s $size
echo "=========="
echo ""
rm aes_key

dd bs=$size count=1 skip=0 if=/dev/urandom of=origin > /dev/null 2> /dev/null

echo "Encryption: "
time build/cipher -i origin -o enc -k keys/enc -e
echo "=========="
echo ""

echo "Decryption: "
time build/cipher -i enc -o dec -k keys/0 -e
echo "=========="
echo ""

rm enc
rm dec
rm origin
rm -rf keys
rm config.xml
