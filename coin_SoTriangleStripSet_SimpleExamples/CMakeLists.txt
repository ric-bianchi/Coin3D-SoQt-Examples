cmake_minimum_required(VERSION 3.7.0)
project(coin_sotrianglestripset_simpleExamples)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Dependencies
find_package( Qt5 REQUIRED COMPONENTS Widgets Core )

# Tell CMake to create the executable
add_executable(coin_sotrianglestripset_simpleExamples main.cpp )

# Tell CMake to use these libraries when linking
target_link_libraries(coin_sotrianglestripset_simpleExamples SoQt Coin Qt5::Widgets)
