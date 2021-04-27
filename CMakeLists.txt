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
    code/gui/HealthBar.cpp 
    code/gui/Tiles.cpp 
    code/entities_and_components/Entity.cpp 
    code/entities_and_components/GraphicsComponent.cpp 
    code/entities_and_components/InputComponent.cpp 
    code/entities_and_components/PhysicsComponent.cpp)

target_include_directories(platformer PUBLIC 
    code/entities_and_components 
    code/gui 
    code/scenes)