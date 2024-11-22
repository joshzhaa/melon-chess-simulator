FetchContent_Declare(googletest URL https://github.com/google/googletest/releases/download/v1.15.2/googletest-1.15.2.tar.gz)
FetchContent_MakeAvailable(googletest)

add_executable(hello_test test_hello.cpp)
target_link_libraries(hello_test GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(hello_test)
