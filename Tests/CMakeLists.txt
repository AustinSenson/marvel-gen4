cmake_minimum_required(VERSION 3.10)

project(marvel-tests)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

set(TEST_EXECUTABLE marvelTest)

enable_language(C ASM)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(UNITY_FILES
    CMock/vendor/unity//src/unity.c)

set(UNITY_DIR
    CMock/vendor/unity/src)

set(TEST_FILES
    ${CMAKE_SOURCE_DIR}/runAllTests.c
    ${CMAKE_SOURCE_DIR}/testCellBalancing.c
    ${CMAKE_SOURCE_DIR}/testChargingAlgorithm.c
    ${CMAKE_SOURCE_DIR}/testContactors.c
    ${CMAKE_SOURCE_DIR}/../Library/Threads/threadxMain.c
    ${CMAKE_SOURCE_DIR}/../Library/Matlab/CoulombCounting/SoCEstimation.c
    # ${CMAKE_SOURCE_DIR}/../Library/Functional/ttfc_lib.c
    )

set(TEST_DIR
    ${CMAKE_SOURCE_DIR}/../Library/Threads
    ${CMAKE_SOURCE_DIR}/fakes
    ${CMAKE_SOURCE_DIR}/mocks
    ${CMAKE_SOURCE_DIR}/../Core/Inc
    ${CMAKE_SOURCE_DIR}/../Library/Interface
    ${CMAKE_SOURCE_DIR}/../Library/Functional
    ${CMAKE_SOURCE_DIR}/../Library/Threads)

add_executable(${TEST_EXECUTABLE}
    ${TEST_FILES}
    ${UNITY_FILES})

target_include_directories(${TEST_EXECUTABLE} PRIVATE
    ${UNITY_DIR}
    ${TEST_DIR})

add_custom_command(TARGET ${TEST_EXECUTABLE}
    POST_BUILD
    COMMAND ./${TEST_EXECUTABLE})
