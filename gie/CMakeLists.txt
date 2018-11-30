cmake_minimum_required(VERSION 3.9)

project(gie_library VERSION 0.0.0 DESCRIPTION "GenerativeImageEditor library")

set(CMAKE_CXX_STANDARD 17)

file(GLOB LIBRARY_SOURCES src/*)

find_package(Boost COMPONENTS python3 REQUIRED)
find_package(PythonLibs REQUIRED)

include_directories(include/ ${Boost_INCLUDE_DIR} ${PYTHON_INCLUDE_DIRS}) # TODO: add libraries to an 3rdparty directory

add_library(gie STATIC ${LIBRARY_SOURCES} include/gie/Graph.h)
target_link_libraries(gie ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})

# TESTS
add_executable(gie_test tests/test.cpp)
target_link_libraries(gie_test gie)