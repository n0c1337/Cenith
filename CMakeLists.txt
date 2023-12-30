cmake_minimum_required(VERSION 3.5)

project(Cenith)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

file(GLOB SOURCES src/*.c)

include_directories(include)

add_executable(cenith ${SOURCES})

add_subdirectory(stdlib)

target_link_libraries(cenith stdlib)