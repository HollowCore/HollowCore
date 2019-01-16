#!/bin/bash -x

source $(dirname $0)/cmake_common_source

cd $(dirname $0)/..

if [[ ! -d ${BUILD_DIR} ]]; then
    $(dirname $0)/build.sh
fi

cd ${BUILD_DIR}
${MAKE} test
