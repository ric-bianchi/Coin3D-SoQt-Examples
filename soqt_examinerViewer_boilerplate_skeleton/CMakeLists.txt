cmake_minimum_required(VERSION 3.7.0)
project(soqt_examinerViewer_boilerplate_skeleton)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Dependencies
find_package( Qt5 REQUIRED COMPONENTS Widgets Core )

# Tell CMake to create the executable
add_executable(soqt_examinerViewer_boilerplate_skeleton boiler.cpp)

# Tell CMake to use these libraries when linking
target_link_libraries(soqt_examinerViewer_boilerplate_skeleton SoQt Coin Qt5::Widgets)
