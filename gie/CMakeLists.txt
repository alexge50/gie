cmake_minimum_required(VERSION 3.9)

project(gie_library VERSION 0.0.0 DESCRIPTION "GenerativeImageEditor library")

set(CMAKE_CXX_STANDARD 17)

set(LIBRARY_SOURCES
        include/gie/Argument.h
        include/gie/DrawableNode.h
        include/gie/execute/Execute.h
        include/gie/Graph.h
        include/gie/LogicNode.h
        include/gie/Node.h
        include/gie/Program.h
        include/gie/PythonContext.h
        include/gie/util/Graph.h
        include/gie/Value.h
        src/util/Graph.cpp
        src/Graph.cpp
        src/Program.cpp
        src/PythonContext.cpp
        )

find_package(Boost COMPONENTS python3 REQUIRED)
find_package(PythonLibs REQUIRED)

include_directories(include/ ${Boost_INCLUDE_DIR} ${PYTHON_INCLUDE_DIRS}) # TODO: add libraries to an 3rdparty directory

add_library(gie STATIC ${LIBRARY_SOURCES})
target_link_libraries(gie ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})

# TESTS
add_executable(gie_test tests/test.cpp)
target_link_libraries(gie_test gie)