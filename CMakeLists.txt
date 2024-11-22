cmake_minimum_required(VERSION 3.25.1)
project(chessmelon VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS off)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

# hack to disable compiler warnings for nlohmann_json
get_target_property(json_include nlohmann_json::nlohmann_json INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(nlohmann_json SYSTEM INTERFACE ${json_include})

add_executable(
  chessmelon
  src/main.cpp
  src/piece.cpp
)
target_include_directories(chessmelon PUBLIC include)
target_link_libraries(chessmelon PUBLIC nlohmann_json::nlohmann_json)
target_compile_options(chessmelon PUBLIC -Weverything -Wno-c++98-compat)

add_subdirectory(test)
