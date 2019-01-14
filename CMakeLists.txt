cmake_minimum_required (VERSION 3.5)
project (HollowCore)

# HollowCore Library
set(SOURCES ${SOURCES} Source/HCCore.c)
set(SOURCES ${SOURCES} Source/HCObject.c)
set(SOURCES ${SOURCES} Source/HCNumber.c)
add_library(hollowcore ${SOURCES})

# Test Suite
set(TESTS ${TESTS} Test/HCCore.c)
set(TESTS ${TESTS} Test/HCNumber.c)
add_executable(hollowcoretest ${TESTS} Test/main.c)
target_link_libraries(hollowcoretest hollowcore)
enable_testing()
add_test(tests hollowcoretest)
