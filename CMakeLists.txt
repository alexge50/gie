cmake_minimum_required(VERSION 3.9)

project(GIE)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif()

add_subdirectory(3rdparty/nodeeditor)
if(NOT TARGET Catch2)
    add_subdirectory(3rdparty/Catch2)
endif()
add_subdirectory(3rdparty/Qt-Color-Widgets)
add_subdirectory(util)
add_subdirectory(gie)
add_subdirectory(modules)
add_subdirectory(gui)

set_target_properties(
        GIE PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)