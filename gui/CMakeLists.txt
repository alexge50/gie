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
find_package(modules HINTS dump)
find_package(KF5WidgetsAddons CONFIG REQUIRED)

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
        src/gie/types/StringData.h
        src/gie/types/NumberData.h
        src/gie/types/TypeData.h
        src/gie/SourceNodeDataModel/StringSourceDataModel.cpp
        src/gie/SourceNodeDataModel/StringSourceDataModel.h
        src/gie/SourceNodeDataModel/NumberSourceDataModel.cpp
        src/gie/SourceNodeDataModel/NumberSourceDataModel.h
        src/gie/SourceNodeDataModel/NumberSourceDataModel.h
        src/gie/types/ColorData.h
        src/serialisation/serialisation.cpp
        src/serialisation/serialisation.h
        src/colorpicker/colorpicker.cpp
        src/colorpicker/colorpicker.h
        src/gie/SourceNodeDataModel/ColorSourceDataModel.cpp
        src/gie/SourceNodeDataModel/ColorSourceDataModel.h
        src/serialisation/serialisation.h
        src/symbolviewer/symbolviewer.cpp
        src/symbolviewer/symbolviewer.h
        src/gie/DisplayNodeDataModel/NumberDisplayDataModel.cpp
        src/gie/DisplayNodeDataModel/NumberDisplayDataModel.h
        src/colorsample/colorsample.cpp
        src/colorsample/colorsample.h
        src/gie/DisplayNodeDataModel/StringDisplayDataModel.cpp
        src/gie/DisplayNodeDataModel/StringDisplayDataModel.h
        src/gie/DisplayNodeDataModel/ColorDisplayDataModel.cpp
        src/gie/DisplayNodeDataModel/ColorDisplayDataModel.h
        src/gie/types/ImageData.h
        src/gie/types/IntegerData.h
        src/gie/SourceNodeDataModel/IntegerSourceDataModel.cpp
        src/gie/SourceNodeDataModel/IntegerSourceDataModel.h
        src/gie/DisplayNodeDataModel/IntegerDisplayDataModel.cpp
        src/gie/DisplayNodeDataModel/IntegerDisplayDataModel.h
        src/filepicker/filepicker.cpp
        src/filepicker/filepicker.h
        src/gie/SourceNodeDataModel/ImageSourceDataModel.cpp
        src/gie/SourceNodeDataModel/ImageSourceDataModel.h
        src/gie/DisplayNodeDataModel/ImageDisplayDataModel.cpp
        src/gie/DisplayNodeDataModel/ImageDisplayDataModel.h
        src/imageviewer/imageviewer.cpp
        src/imageviewer/imageviewer.h
        src/Project.cpp src/Project.h
        src/newproject/newproject.cpp
        src/newproject/newproject.h
        src/newproject/newproject.ui
        src/directorypicker/directorypicker.cpp
        src/directorypicker/directorypicker.h
        src/gie/DisplayNodeDataModel/PreviewImageDisplayDataModel.cpp
        src/gie/DisplayNodeDataModel/PreviewImageDisplayDataModel.h
        src/importimage/importimage.cpp
        src/importimage/importimage.h
        src/importimage/importimage.ui
)


add_definitions(-DQT_NO_KEYWORDS)

add_executable(gui ${SOURCE})
target_link_libraries(gui Qt5::Widgets gie::gie NodeEditor::nodes KF5::WidgetsAddons)
target_include_directories(gui PRIVATE gie::gie NodeEditor::nodes modules::modules KF5::WidgetsAddons)
target_compile_definitions(gui PRIVATE NODE_EDITOR_SHARED)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(gui PRIVATE -Wall -Werror -fcompare-debug -g)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    target_compile_options(gui PRIVATE  -Wall -Werror -g -fstandalone-debug)
endif()

file(INSTALL dump/modules DESTINATION ./)