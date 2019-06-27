#!/bin/bash

###################################################################
# Setup
###################################################################
function run_test () {
    $(dirname $0)/test.sh
}

function run_valgrind_test () {
    $(dirname $0)/valgrind_test.sh
}

###################################################################
# Main Execution
###################################################################
if [[ ${NAME} == "Test" ]]; then
    run_test
elif [[ ${NAME} == "ValgrindTest" ]]; then
    run_valgrind_test
fi
