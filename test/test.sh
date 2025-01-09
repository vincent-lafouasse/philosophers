#!/bin/bash

make >/dev/null

GREEN='\033[0;32m'
RED='\033[0;31m'
PURPLE='\033[0;35m'
NC='\033[0m'

should_complete() {
    echo "${GREEN}=====SHOULD COMPLETE=====$NC"
    sleep 1
}

should_continue() {
    echo "${PURPLE}=====SHOULD GO ON ENDLESSLY=====$NC"
    sleep 1
}

should_die() {
    echo "${RED}=====SHOULD DIE=====$NC"
    sleep 1
}

if [ $# -eq 0 ]; then
    echo "Usage: sh test/test.sh [1-5]"
    exit
fi

if [ "$1" == "1" ]; then
    should_die
    ./philo 1 800 200 200
elif [ "$1" == "2" ]; then
    should_continue
    ./philo 5 800 200 200
elif [ "$1" == "3" ]; then
    should_complete
    ./philo 5 800 200 200 7
elif [ "$1" == "4" ]; then
    should_continue
    ./philo 4 410 200 200
elif [ "$1" == "5" ]; then
    should_die
    ./philo 4 310 200 100
else
    echo "Usage: sh test/test.sh [1-5]"
    exit
fi
