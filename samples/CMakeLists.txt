cmake_minimum_required(VERSION 3.14)
project(samples)

file(GLOB SOURCES *.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE test test_main)