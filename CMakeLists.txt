cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
project(hello_world)

set(SOURCE_EXE main.cpp)
set(SOURCE_LIB hello.cpp)

add_library(hello STATIC ${SOURCE_LIB})
add_executable(main ${SOURCE_EXE})

target_link_libraries(main hello)
