cmake_minimum_required(VERSION 3.10)
include(FetchContent)

project(HelloWorld)

set(PN ${PROJECT_NAME})

file(GLOB_RECURSE ${PN}_HEADERS
    "include/*.h"
)
file(GLOB_RECURSE ${PN}_SRC
    "src/*.cpp"
)

add_executable(${PN} main.cpp)