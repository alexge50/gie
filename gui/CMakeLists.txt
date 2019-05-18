cmake_minimum_required(VERSION 3.9)
project(gui)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt5Core)

set(SOURCE
        mainwindow.ui
        mainwindow.cpp
        main.cpp
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")

add_executable(gui ${SOURCE})
target_link_libraries(gui Qt5::Widgets)
