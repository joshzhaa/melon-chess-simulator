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

# only supports clang++
add_compile_options(
  -Weverything
  -Wno-c++98-compat
  -Wno-c++98-compat-pedantic
  -Wno-c++20-compat
  -Wno-pre-c++20-compat-pedantic
  -Wno-global-constructors
  -Wno-exit-time-destructors
  -Wno-switch-default
  -Wno-unsafe-buffer-usage
)

add_library(melon STATIC src/game.cpp src/piece.cpp src/traits.cpp)
target_include_directories(melon PUBLIC include)
target_link_libraries(melon PUBLIC nlohmann_json::nlohmann_json)

add_executable(melon_cli src/cli/main.cpp src/cli/text_io.cpp)
target_include_directories(melon_cli PUBLIC include)
target_link_libraries(melon_cli PUBLIC melon nlohmann_json::nlohmann_json)

add_executable(test_math test/math.cpp)
target_link_libraries(test_math GTest::gtest_main)  # math is header only
target_include_directories(test_math PUBLIC include)

add_executable(test_piece test/piece.cpp src/cli/text_io)  # TODO: remove text_io
target_link_libraries(test_piece GTest::gtest_main melon)
target_include_directories(test_piece PUBLIC include)

enable_testing()
include(GoogleTest)
gtest_discover_tests(test_math)
gtest_discover_tests(test_piece)
