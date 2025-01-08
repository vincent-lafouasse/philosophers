#!/bin/bash

make >/dev/null

if [ $# -eq 0 ]; then
    echo "Usage: sh test/test.sh [1-5]"
    exit
fi

if [ "$1" == "1" ]; then
    echo test 1
else
    echo "Usage: sh test/test.sh [1-5]"
    exit
fi
