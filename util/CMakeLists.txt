cmake_minimum_required(VERSION 3.9)

project(util)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS include/*)

add_library(util INTERFACE)
target_include_directories(util INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/)
install(
        TARGETS util
        EXPORT util-config
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(
        EXPORT util-config
        NAMESPACE util::
        DESTINATION lib/cmake/util)
