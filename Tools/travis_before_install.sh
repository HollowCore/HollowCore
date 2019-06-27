#!/bin/bash

if [[ $TRAVIS_OS_NAME == "linux" ]]; then
    sudo apt install valgrind
fi
