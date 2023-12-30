cmake_minimum_required(VERSION 3.5)

file(GLOB SOURCES src/*.c)
file(GLOB HEADERS include/*.h)

include_directories(include)

add_library(stdlib ${SOURCES} ${HEADERS})

target_include_directories(stdlib PUBLIC include)