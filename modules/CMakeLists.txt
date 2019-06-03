cmake_minimum_required(VERSION 3.9)

project(modules CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Boost COMPONENTS python3 REQUIRED)
find_package(PythonLibs REQUIRED)

python_add_module(internals src/internals.cpp)
target_link_libraries(internals ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
target_include_directories(internals PRIVATE include/ ${Boost_INCLUDE_DIR} ${PYTHON_INCLUDE_DIRS})

python_add_module(images_internal src/images_internal.cpp)
target_link_libraries(images_internal ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
target_include_directories(images_internal PRIVATE include/ ${Boost_INCLUDE_DIR} ${PYTHON_INCLUDE_DIRS})

set_target_properties(
        internals PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/modules"
)

set_target_properties(
        images_internal PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/modules"
)

add_custom_target(
        modules
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/modules" "${CMAKE_BINARY_DIR}/bin/modules"
        DEPENDS modules
        COMMENT "copying python modules"
        VERBATIM
)

add_library(gie-types INTERFACE)
include(GNUInstallDirs)

target_include_directories(
        gie-types INTERFACE
       $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
       $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

install(
        TARGETS gie-types
        EXPORT gie-types-config)

install(
        EXPORT gie-types-config
        NAMESPACE gie::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/modules)

install(
        DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/)