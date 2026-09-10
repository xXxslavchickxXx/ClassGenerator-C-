find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(${PN}_main main.cpp)
target_link_libraries(${PN}_main
    PRIVATE
        ${PN}_LIB
        GTest::gtest_main
        GTest::gtest
)

enable_testing()
gtest_discover_tests(${PN}_main)