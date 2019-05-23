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
        src/gie/StringData.h
        src/gie/NumberData.h
        src/gie/TypeData.h
        src/gie/SourceNodeDataModel/StringSourceDataModel.cpp
        src/gie/SourceNodeDataModel/StringSourceDataModel.h
        src/gie/SourceNodeDataModel/NumberSourceDataModel.cpp
        src/gie/SourceNodeDataModel/NumberSourceDataModel.h
        src/serialisation/serialise.h
        src/serialisation/deserialise.h)

add_definitions(-DQT_NO_KEYWORDS)

add_executable(gui ${SOURCE})
target_link_libraries(gui Qt5::Widgets gie::gie NodeEditor::nodes)
target_include_directories(gui PRIVATE gie::gie NodeEditor::nodes)
target_compile_definitions(gui PRIVATE NODE_EDITOR_SHARED)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(gui PRIVATE -Wall -Werror -fcompare-debug -g)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    target_compile_options(gui PRIVATE  -Wall -Werror -g -fstandalone-debug)
endif()

file(INSTALL modules DESTINATION ./)