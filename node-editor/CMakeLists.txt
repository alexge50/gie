cmake_minimum_required(VERSION 3.9)
project(node-editor)

find_package(Freetype REQUIRED)
find_package (Python3 COMPONENTS Interpreter)

function(add_shader target vertex fragment)
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/shader/)
    add_custom_target(
            ${target}_gen
            COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/scripts/shader.py ${CMAKE_CURRENT_SOURCE_DIR}/${vertex} ${CMAKE_CURRENT_SOURCE_DIR}/${fragment} ${CMAKE_CURRENT_BINARY_DIR}/shader/${target}.h
            BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/shader/${target}.h
            WORKING_DIRECTORY ..
            DEPENDS ${vertex} ${fragment}
    )

    add_library(${target} INTERFACE)
    target_include_directories(${target} INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/shader/)
    add_dependencies(${target} ${target}_gen)
endfunction()

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)

file(GLOB_RECURSE SHADERS CONFIGURE_DEPENDS shaders/*)
add_shader(shader_solid shaders/solid.vertex.glsl shaders/solid.fragment.glsl)
add_shader(shader_background shaders/background.vertex.glsl shaders/background.fragment.glsl)
add_shader(shader_text shaders/text.vertex.glsl shaders/text.fragment.glsl)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*)
add_library(node_editor ${SOURCES})
target_link_libraries(node_editor PRIVATE glad OpenGL::GL)
target_link_libraries(node_editor PUBLIC glm shader_solid shader_background shader_text Freetype::Freetype)
target_include_directories(node_editor PUBLIC include/)