find_package(GTest REQUIRED)
include(GoogleTest)

file(GLOB_RECURSE ${PN}_test_src
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)
file(GLOB_RECURSE ${PN}_test_hdrs
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)
list(REMOVE_ITEM ${PN}_test_src "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
message("[INFORMATION] ${${PN}_test_src}")
add_executable(${PN}_tests)
target_sources(${PN}_tests
    PRIVATE
        ${${PN}_test_src}
)

# If tests have a .h files
if (${PN}_test_hdrs)
    target_sources(${PN}_tests
        PRIVATE
            FILE_SET HEADERS
            BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
            FILES ${${PN}_test_hdrs}
    )
endif()

target_link_libraries(${PN}_tests
    PRIVATE
        ${PN}_LIB
        GTest::gtest_main
        GTest::gtest
)

gtest_discover_tests(${PN}_tests)