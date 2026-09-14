find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(${PN}_main main.cpp)
target_link_libraries(${PN}_main
    PRIVATE
        ${PN}_LIB
)

file(GLOB_RECURSE ${PN}_test_src
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)
list(REMOVE_ITEM ${PN}_test_src "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
message("[INFORMATION] ${${PN}_test_src}")
add_executable(${PN}_tests)
target_sources(${PN}_tests
    PRIVATE
        ${${PN}_test_src}
)
target_link_libraries(${PN}_tests
    PRIVATE
        ${PN}_LIB
        GTest::gtest_main
        GTest::gtest
)

enable_testing()
gtest_discover_tests(${PN}_tests)