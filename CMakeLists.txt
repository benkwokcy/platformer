cmake_minimum_required(VERSION 3.5)
project(platformer LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -Wpedantic -Wshadow)

# Sanitizers
add_compile_options(-fsanitize=address -fsanitize=undefined)
add_link_options(-fsanitize=address -fsanitize=undefined)

# Libraries
add_link_options(-lSDL2 -lSDL2_image -ltinyxml2)

add_executable(platformer 
    code/Main.cpp 
    code/scenes/Level.cpp 
    code/core/Tiles.cpp 
    code/entities/ui/HealthBar.cpp 
    code/entities/characters/Character.cpp 
    code/entities/characters/GraphicsComponent.cpp 
    code/entities/characters/InputComponent.cpp 
    code/entities/characters/PhysicsComponent.cpp)

target_include_directories(platformer PUBLIC 
    code/entities/characters
    code/entities/objects 
    code/entities/ui 
    code/core 
    code/scenes
    code/utils)