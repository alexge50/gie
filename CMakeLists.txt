cmake_minimum_required(VERSION 3.14)

project(GIE)

add_subdirectory(3rdparty/nodeeditor)
add_subdirectory(3rdparty/Qt-Color-Widgets)
add_subdirectory(gie)
add_subdirectory(modules)
add_subdirectory(gui)

set_target_properties(
        GIE PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)