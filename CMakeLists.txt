cmake_minimum_required(VERSION 3.7)
project(CHIP8_Emulator)

set(SOURCE_FILES src/emulator.cpp src/system.cpp)
add_executable(emulator ${SOURCE_FILES})

# SDL2 required for rendering and taking keypress inputs
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

target_link_libraries(emulator ${SDL2_LIBRARIES})