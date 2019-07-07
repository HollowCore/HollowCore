#!/bin/bash

###################################################################
# Setup
###################################################################
function before_linux () {
    if [[ ${NAME} == "ValgrindTest" ]]; then
        sudo apt-get update
        sudo apt-get install libc6-dbg
        sudo apt-get install -f valgrind
    fi
}

###################################################################
# Main Execution
###################################################################
if [[ $TRAVIS_OS_NAME == "linux" ]]; then
    before_linux
fi
