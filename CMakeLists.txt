cmake_minimum_required(VERSION 3.5)

project(platformer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler Warnings
add_compile_options(-Wall -Wextra -Wpedantic -Wshadow)

# Sanitizers (comment out for release builds)
add_compile_options(-g -O1 -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -fno-optimize-sibling-calls)
add_link_options(-fsanitize=address -fsanitize=undefined)

# SDL
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(platformer main.cpp)
target_link_libraries(platformer ${SDL2_LIBRARIES})
