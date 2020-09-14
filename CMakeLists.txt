cmake_minimum_required(VERSION 3.9)
project(node-editor)

function(add_resource target)
    set(generated_file ${CMAKE_CURRENT_BINARY_DIR}/${target}/${target}.h)
    file(WRITE ${generated_file})
    foreach(input ${ARGN})
        string(REGEX MATCH "([^/]+)$" filename ${input})
        string(REGEX REPLACE "\\.| |-" "_" filename ${filename})
        file(READ ${input} data HEX)
        string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," data ${data})
        file(
                APPEND ${generated_file}
                "const unsigned char ${filename}[]={${data}0x00};\nconst unsigned ${filename}_size = sizeof(${filename});\n"
        )
    endforeach()
    add_library(${target} INTERFACE)
    target_include_directories(${target} INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/${target}/)
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