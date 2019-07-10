cmake_minimum_required(VERSION 3.7.0)
project(soqt_customExaminerViewer)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Dependencies
find_package(Qt5 REQUIRED COMPONENTS Widgets Core)


# Tell CMake to create the helloworld executable
add_executable(soqt_customExaminerViewer customViewer.h customViewer.cpp main.cpp)

# Tell CMake to use these libraries when linking
target_link_libraries(soqt_customExaminerViewer Coin SoQt Qt5::Widgets)
