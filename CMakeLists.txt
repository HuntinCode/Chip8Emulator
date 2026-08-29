cmake_minimum_required(VERSION 3.16)
project(Chip8Emu)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

add_executable(Chip8Emu src/main.cpp src/Chip8.cpp src/Chip8_Opcodes.cpp)

target_link_libraries(Chip8Emu PRIVATE SDL3::SDL3)