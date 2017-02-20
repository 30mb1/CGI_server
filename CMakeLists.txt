cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c99 -Wall")
project(CGI_server)

set(SOURCE_EXE main.c)

set(SOURCE_LIB utility.c)

add_library(utility STATIC ${SOURCE_LIB})

add_executable(main ${SOURCE_EXE})

target_link_libraries(main utility)
