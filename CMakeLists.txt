cmake_minimum_required(VERSION 3.14)
project(test)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCES test/*.cpp)

add_library(test ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(test_main main.cpp)
target_link_libraries(test_main PUBLIC test)

option(ENABLE_SAMPLES "Enable all samples for this library" OFF)

if(ENABLE_SAMPLES)
    add_subdirectory(samples)
endif()
