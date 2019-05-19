cmake_minimum_required(VERSION 3.9)
project(gui)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt5Core)
find_package(gie REQUIRED HINTS dump)
find_package(NodeEditor REQUIRED HINTS dump)

set(SOURCE
        src/mainwindow.ui
        src/mainwindow.cpp
        src/mainwindow.h
        src/main.cpp
        src/editor.h
        src/editor.cpp
        src/gie/GieDataModelRegistry.cpp
        src/gie/GieDataModelRegistry.h
        src/gie/GieNodeDataModel.cpp
        src/gie/GieNodeDataModel.h
        src/gie/StringData.h src/gie/NumberData.h src/gie/TypeData.h)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")

add_executable(gui ${SOURCE})
target_link_libraries(gui Qt5::Widgets gie::gie NodeEditor::nodes)
target_include_directories(gui PRIVATE gie::gie NodeEditor::nodes)
target_compile_definitions(gui PRIVATE NODE_EDITOR_SHARED)

file(INSTALL modules DESTINATION ./)