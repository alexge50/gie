cmake_minimum_required(VERSION 3.9)

project(modules CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Boost COMPONENTS python3 REQUIRED)
find_package(PythonLibs REQUIRED)

python_add_module(internals src/internals.cpp)
target_link_libraries(internals ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
target_include_directories(internals PRIVATE include/ ${Boost_INCLUDE_DIR} ${PYTHON_INCLUDE_DIRS})

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(internals PRIVATE -O3)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    target_compile_options(internals PRIVATE -O3)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    target_compile_options(internals PRIVATE /MD /Ob2)
endif()

python_add_module(images_internal src/images_internal.cpp)
target_link_libraries(images_internal ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
target_include_directories(images_internal PRIVATE include/ ${Boost_INCLUDE_DIR} ${PYTHON_INCLUDE_DIRS})

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(images_internal PRIVATE -O3)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    target_compile_options(images_internal PRIVATE -O3)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    target_compile_options(images_internal PRIVATE /MD /Ob2)
endif()

install(TARGETS internals images_internal DESTINATION modules)
install(FILES modules/arithmetic.py modules/colors.py modules/string.py modules/images.py modules/blur.py modules/color_correction.py modules/effects.py modules/noise.py DESTINATION modules)

add_library(modules INTERFACE)
include(GNUInstallDirs)

target_include_directories(
       modules INTERFACE include/
       $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

install(
        TARGETS modules
        EXPORT modules-config)

install(
        EXPORT modules-config
        NAMESPACE modules::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/modules)

install(
        DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/modules)