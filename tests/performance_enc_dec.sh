#!/bin/bash

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

dd bs=8388608 count=1 skip=0 if=/dev/urandom of=origin > /dev/null 2> /dev/null

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
