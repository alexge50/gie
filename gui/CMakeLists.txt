cmake_minimum_required(VERSION 3.9)
project(gui)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt5Core)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*)
file(GLOB_RECURSE RESOURCES CONFIGURE_DEPENDS resources/*)

set(SOURCE ${SOURCES} ${RESOURCES})

add_definitions(-DQT_NO_KEYWORDS)

add_executable(GIE ${SOURCE})
add_dependencies(GIE modules internals images_internal)
target_link_libraries(GIE PRIVATE Qt5::Widgets gie nodes gie-types QtColorWidgets)
target_include_directories(GIE PRIVATE gie nodes gie-types QtColorWidgets)
target_compile_definitions(GIE PRIVATE NODE_EDITOR_SHARED)

add_custom_command(
        TARGET GIE POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/config $<TARGET_FILE_DIR:GIE>
)