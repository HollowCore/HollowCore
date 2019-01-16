#!/bin/bash

########################################
# Setup
########################################
source $(dirname $0)/cmake_common_source
IS_CLEAN=false

########################################
# Functions
########################################
function show_help () {
    echo "OVERVIEW: Helper script to build Hollow Core"
    echo ""
    echo "USAGE: $(basename $0) [options]"
    echo ""
    echo "OPTIONS:"
    echo "  -h | --help | -?"
    echo "              Print this message."
    echo "  --clean     Remove build artifacts before building."
    echo ""
}

########################################
# Parse Args
########################################
while [[ $1 == -* ]]; do
    case "$1" in
    -h|--help|-\?)
        show_help
        exit 0
        ;;
    --clean)
        IS_CLEAN=true
        shift
        ;;
    --)
        shift
        break
        ;;
    -*)
        echo "Invalid option: $1" 1>&2
        show_help
        exit 1
        ;;
    esac
done

########################################
# Main Execution
########################################
cd $(dirname $0)/..

if [[ ${IS_CLEAN} == true ]]; then
    rm -rf ${BUILD_DIR}
fi

mkdir -p ${BUILD_DIR}
cd ${BUILD_DIR}
${CMAKE} ..
${MAKE}
