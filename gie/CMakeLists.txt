cmake_minimum_required(VERSION 3.9)

project(gie_library VERSION 0.0.0 DESCRIPTION "GenerativeImageEditor library")

set(CMAKE_CXX_STANDARD 17)

set(LIBRARY_SOURCES
        include/gie/Argument.h
        include/gie/NodeDrawable.h
        include/gie/ScriptGraph/Execute.h
        include/gie/ScriptGraph/ScriptGraph.h
        include/gie/NodeLogic.h
        include/gie/Node.h
        include/gie/Program.h
        include/gie/PythonContext.h
        include/gie/util/Graph.h
        include/gie/Value.h
        src/Program.cpp
        src/PythonContext.cpp
        src/ScriptGraph/Execute.cpp
        src/ScriptGraph/ScriptGraph.cpp
        include/gie/NodeUtil.h
        src/NodeUtil.cpp)


find_package(Boost COMPONENTS python3 graph REQUIRED)
find_package(PythonLibs REQUIRED)

include_directories(include/ ${Boost_INCLUDE_DIR} ${PYTHON_INCLUDE_DIRS}) # TODO: add libraries to an 3rdparty directory

add_library(gie STATIC ${LIBRARY_SOURCES})
target_link_libraries(gie ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
target_compile_options(gie PRIVATE -Wall -Wextra -pedantic -Werror)
target_include_directories(gie PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/{include_dir}> /
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

install (TARGETS gie
        LIBRARY DESTINATION lib)
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/
        DESTINATION include)

# TESTS
find_package(Catch2 REQUIRED)

add_executable(gie_test tests/test.cpp)
target_link_libraries(gie_test gie)

file(INSTALL tests/test_modules DESTINATION ./)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(gie_test PRIVATE -fcompare-debug)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    target_compile_options(gie_test PRIVATE -fstandalone-debug)
endif()

# add_executable(gie_dev_test tests/dev_test.cpp)
# target_link_libraries(gie_dev_test gie)