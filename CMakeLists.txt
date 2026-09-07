cmake_minimum_required(VERSION 3.25)
include(FetchContent)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/LibCreator.cmake)

project(HelloWorld)

set(PN ${PROJECT_NAME})

file(GLOB_RECURSE ${PN}_HEADERS
    "include/*.h"
)
file(GLOB_RECURSE ${PN}_SRC
    "src/*.cpp"
)

# add_library(${PN}_LIB)

# target_sources(${PN}_LIB
#     PRIVATE
#         ${${PN}_SRC}
    
#     PUBLIC
#         FILE_SET HEADERS
#         BASE_DIRS
#             ${CMAKE_CURRENT_SOURCE_DIR}/include
#         FILES
#             ${${PN}_HEADERS}
# )

create_lib(${PN}_LIB    
    PUBLIC
        FILE_SET HEADERS
        FILES
            ${${PN}_HEADERS}
        BASE_DIRS
            ${CMAKE_CURRENT_SOURCE_DIR}/include

    PRIVATE
        ${${PN}_SRC}
)

add_executable(${PN} main.cpp)

target_link_libraries(${PN} PRIVATE ${PN}_LIB)