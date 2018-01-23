cmake_minimum_required(VERSION 3.7.0)
project(soqt-customExaminerViewer)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets)


find_package(Qt5 REQUIRED COMPONENTS Widgets Core)
find_package(Coin REQUIRED)
find_package(SoQt REQUIRED)



message( "QT_VERSION: " ${QT_VERSION})
message( "Qt5_LIBRARIES: " ${Qt5_LIBRARIES})
message( "Qt5_INCLUDE_DIRS: " ${Qt5_INCLUDE_DIRS})
message( "SoQt_LIBRARIES: " ${SoQt_LIBRARIES} )
message( "SoQt_INCLUDE_DIRS: "  ${SoQt_INCLUDE_DIRS})
message( "Coin_LIBRARY: " ${Coin_LIBRARY})
message( "Coin_INCLUDE_DIR: " ${Coin_INCLUDE_DIR}) 


# Tell CMake to create the helloworld executable
add_executable(customExaminerViewer customViewer.h customViewer.cpp main.cpp)

# Use the Widgets module from Qt 5.
target_link_libraries(customExaminerViewer ${SoQt_LIBRARIES} Qt5::Widgets)
