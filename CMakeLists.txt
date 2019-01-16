cmake_minimum_required (VERSION 3.5)
project (HollowCore)

# Configure Environment
if (COVERAGE)
    find_program(GCOV_PATH gcov)
    if(NOT GCOV_PATH)
        message(FATAL_ERROR "gcov not found! Aborting...")
    endif()

    find_program(LCOV_PATH  NAMES lcov lcov.bat lcov.exe lcov.perl)
    if(NOT LCOV_PATH)
        message(FATAL_ERROR "lcov not found! Aborting...")
    endif()

    find_program(GENHTML_PATH NAMES genhtml genhtml.perl genhtml.bat)
    if(NOT GENHTML_PATH)
        message(FATAL_ERROR "genhtml not found! Aborting...")
    endif()

    set(CMAKE_C_FLAGS "-g -O0 -Wall -W -fprofile-arcs -ftest-coverage")
    set(CMAKE_CXX_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage")
    set(CMAKE_LD_FLAGS "-fprofile-arcs -ftest-coverage")
    set(CMAKE_C_OUTPUT_EXTENSION_REPLACE ON)
    set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)
endif ()

# HollowCore Library
set(HOLLOWCORE_LIBRARY_NAME hollowcore)
set(SOURCES ${SOURCES} Source/HCCore.c)
set(SOURCES ${SOURCES} Source/HCObject.c)
set(SOURCES ${SOURCES} Source/HCNumber.c)
add_library(${HOLLOWCORE_LIBRARY_NAME} ${SOURCES})

# HollowCore Links
target_link_libraries(${HOLLOWCORE_LIBRARY_NAME} m)

# HollowCore Test Suite
set(HOLLOWCORE_TEST_NAME hollowcoretest)
set(TEST_SOURCES ${TEST_SOURCES} Test/main.c)
set(TEST_SOURCES ${TEST_SOURCES} Test/HCCore.c)
set(TEST_SOURCES ${TEST_SOURCES} Test/HCNumber.c)
add_executable(${HOLLOWCORE_TEST_NAME} ${TEST_SOURCES})
target_link_libraries(${HOLLOWCORE_TEST_NAME} hollowcore)
enable_testing()
add_test(tests ${HOLLOWCORE_TEST_NAME})

# HollowCore Test Suite Code Coverage
if (COVERAGE)
    set(OBJECT_DIR ${CMAKE_BINARY_DIR}/CMakeFiles/${HOLLOWCORE_LIBRARY_NAME}.dir/Source)
    add_custom_target(coverage
        COMMAND mkdir -p coverage
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
    add_custom_command(TARGET coverage
        COMMAND echo "-- LCOV establishing baseline..."
        COMMAND ${LCOV_PATH} --gcov-tool ${GCOV_PATH} -d ${OBJECT_DIR} -z
        COMMAND ${LCOV_PATH} --gcov-tool ${GCOV_PATH} -d ${OBJECT_DIR} -c -i -o coverage.base
        COMMAND echo "-- LCOV baseline established"
        COMMAND echo "-- LCOV running tests..."
        COMMAND ${CMAKE_BINARY_DIR}/${HOLLOWCORE_TEST_NAME}
        COMMAND echo "-- LCOV tests complete"
        COMMAND echo "-- LCOV generating coverage..."
        COMMAND ${LCOV_PATH} --gcov-tool ${GCOV_PATH} -d ${OBJECT_DIR} -c -o coverage.info
        COMMAND ${LCOV_PATH} --gcov-tool ${GCOV_PATH} -a coverage.base -a coverage.info -o coverage.total
        COMMAND ${GENHTML_PATH} coverage.total --output-directory html
        COMMAND echo "-- LCOV coverage generation complete"
        COMMAND echo "-- GCOV analysis"
        COMMAND ${GCOV_PATH} ${SOURCES} -o ${OBJECT_DIR} > summary.txt
        COMMAND cat summary.txt
        COMMAND echo "-- Coverage files have been output to ${CMAKE_BINARY_DIR}/coverage"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/coverage
    )
    add_dependencies(coverage ${HOLLOWCORE_TEST_NAME})
    set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES coverage)
endif ()