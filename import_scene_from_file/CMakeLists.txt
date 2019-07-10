cmake_minimum_required(VERSION 3.7.0)
project(import_scene_from_file)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Dependencies
find_package(Qt5 REQUIRED COMPONENTS Widgets Core)

# Tell CMake to create the helloworld executable
add_executable(import_scene_from_file main.cpp)

# Tell CMake to use these libraries when linking
target_link_libraries(import_scene_from_file Coin SoQt Qt5::Widgets)

# Tell CMake to copy the data file to the build folder, after compilation
add_custom_command(
        TARGET import_scene_from_file POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/data/test.iv
                ${CMAKE_CURRENT_BINARY_DIR}/data/test.iv)
