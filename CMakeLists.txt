cmake_minimum_required(VERSION 3.14)

project(GIE)

add_subdirectory(3rdparty/nodeeditor)
add_subdirectory(3rdparty/Qt-Color-Widgets)
add_subdirectory(gie)
add_subdirectory(modules)
add_subdirectory(gui)

add_custom_command(
        DEPENDS internals images_internal
        OUTPUT empty.x
)

file(COPY ${CMAKE_BINARY_DIR}/modules/modules DESTINATION ${CMAKE_BINARY_DIR}/gui/)