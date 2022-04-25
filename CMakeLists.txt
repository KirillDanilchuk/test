cmake_minimum_required(VERSION 3.14)
project(test)

set(CMAKE_CXX_STANDARD 20)

add_library(${PROJECT_NAME} test/test.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(test_main main.cpp)
target_link_libraries(test_main PUBLIC ${PROJECT_NAME})
