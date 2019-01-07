cmake_minimum_required (VERSION 3.5)
project (HollowCore)

set(SOURCES ${SOURCES} HCCore.c)
set(SOURCES ${SOURCES} HCObject.c)
set(SOURCES ${SOURCES} HCNumber.c)
add_library(hollowcore ${SOURCES})
