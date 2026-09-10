cmake_minimum_required(VERSION 3.25)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/LibCreator.cmake)

project(HelloWorld)

set(PN ${PROJECT_NAME})

file(GLOB_RECURSE ${PN}_HEADERS
    CONFIGURE_DEPENDS 
        "include/*.h"
)
file(GLOB_RECURSE ${PN}_SRC
    CONFIGURE_DEPENDS
        "src/*.cpp"
)

create_lib(${PN}_LIB
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS
            ${CMAKE_CURRENT_SOURCE_DIR}/include
        FILES
            ${${PN}_HEADERS}

    PRIVATE
        ${${PN}_SRC}
)

if (BUILD_TESTS)
    add_subdirectory(tests)
endif()