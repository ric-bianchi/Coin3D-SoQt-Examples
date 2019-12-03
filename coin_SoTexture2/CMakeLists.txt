cmake_minimum_required(VERSION 3.7.0)
project(coin_ex_sotexture2)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Dependencies
find_package( Coin REQUIRED )
find_package( SoQt REQUIRED )
find_package( Qt5 REQUIRED COMPONENTS Widgets Core )

# Tell CMake to create the executable
add_executable(coin_ex_sotexture2 main.cpp)

# Tell CMake to use these libraries when linking
target_link_libraries(coin_ex_sotexture2 SoQt Coin Qt5::Widgets)

add_custom_command(
        TARGET coin_ex_sotexture2 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/grass.jpg
                ${CMAKE_CURRENT_BINARY_DIR}/grass.jpg)
