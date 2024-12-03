cmake_minimum_required(VERSION 3.25.1)
project(melon VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS off)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_Declare(googletest URL https://github.com/google/googletest/releases/download/v1.15.2/googletest-1.15.2.tar.gz)
FetchContent_MakeAvailable(json googletest)

# hack to disable compiler warnings for nlohmann_json
get_target_property(json_include nlohmann_json::nlohmann_json INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(nlohmann_json SYSTEM INTERFACE ${json_include})

add_executable(
  melon_cli
  src/cli/main.cpp
  src/cli/text_io.cpp
  src/game.cpp
  src/piece.cpp
  src/traits.cpp
)
target_include_directories(melon_cli PUBLIC include)
target_link_libraries(melon_cli PUBLIC nlohmann_json::nlohmann_json)
target_compile_options(
  melon_cli PUBLIC
  -Weverything
  -Wno-c++98-compat
  -Wno-c++20-compat
  -Wno-pre-c++20-compat-pedantic
  -Wno-exit-time-destructors
  -Wno-switch-default
)

add_executable(test_math test/math.cpp)
target_link_libraries(test_math GTest::gtest_main)
target_include_directories(test_math PUBLIC include)

add_executable(test_traits test/traits.cpp)
target_link_libraries(test_traits GTest::gtest_main)
target_include_directories(test_traits PUBLIC include)

include(GoogleTest)
gtest_discover_tests(test_math test_traits)
