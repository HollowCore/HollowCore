cmake_minimum_required (VERSION 3.5)
project (HollowCore)

set(SOURCES ${SOURCES} Source/HCCore.c)
set(SOURCES ${SOURCES} Source/HCObject.c)
set(SOURCES ${SOURCES} Source/HCNumber.c)
add_library(hollowcore ${SOURCES})
