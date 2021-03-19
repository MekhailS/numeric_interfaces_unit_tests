cmake_minimum_required(VERSION 3.17)
project(numeric_interfaces_unit_tests)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(gtest)
include_directories(include src)

set(INCLUDE
        ${DIR_INCLUDE}/IVector.h
        ${DIR_INCLUDE}/ILogger.h
        ${DIR_INCLUDE}/RC.h
        )

add_executable(numeric_interfaces_unit_tests main.cpp)
