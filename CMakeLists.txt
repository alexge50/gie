cmake_minimum_required(VERSION 3.9)
project(node-editor)

find_package (Python3 COMPONENTS Interpreter)

function(add_resource target)
    add_custom_target(
            ${target}_gen
            COMMAND ${Python3_EXECUTABLE} ${PROJECT_SOURCE_DIR}/scripts/resource.py ${CMAKE_CURRENT_BINARY_DIR}/${target}/${target}.h ${ARGN}
            BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/${target}/${target}.h
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            DEPENDS ${ARGN}
    )

    add_library(${target} INTERFACE)
    target_include_directories(${target} INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/${target}/)
    add_dependencies(${target} ${target}_gen)
endfunction()

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(3rdparty/glfw)
add_subdirectory(3rdparty/glad)
add_subdirectory(3rdparty/glm)

find_package(OpenGL REQUIRED)

add_resource(shaders shaders/background_fragment.glsl shaders/background_vertex.glsl)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*)
add_executable(node_editor ${SOURCES})
target_link_libraries(node_editor PRIVATE glfw glad glm OpenGL::GL shaders)
target_include_directories(node_editor PRIVATE include/)