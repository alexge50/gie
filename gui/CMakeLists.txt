cmake_minimum_required(VERSION 3.9)
project(gui)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*)
add_executable(GIE ${SOURCES})
target_link_libraries(GIE PRIVATE node_editor glfw glad)
target_include_directories(node_editor PRIVATE include)