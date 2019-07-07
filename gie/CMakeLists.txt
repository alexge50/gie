cmake_minimum_required(VERSION 3.9)

project(gie_library VERSION 0.0.0 DESCRIPTION "GenerativeImageEditor library")

set(CMAKE_CXX_STANDARD 17)
include(GNUInstallDirs)

set(LIBRARY_SOURCES
        include/gie/Argument.h
        include/gie/NodeDrawable.h
        include/gie/ScriptGraph/Execute.h
        include/gie/ScriptGraph/ScriptGraph.h
        include/gie/NodeLogic.h
        include/gie/Node.h
        src/Node.cpp
        include/gie/Program.h
        include/gie/PythonContext.h
        include/gie/Value.h
        src/Program.cpp
        src/PythonContext.cpp
        src/ScriptGraph/Execute.cpp
        src/ScriptGraph/ScriptGraph.cpp

        include/gie/Result.h
        src/NodeTypeManager.cpp include/gie/NodeTypeManager.h include/gie/NodeType.h src/NodeType.cpp
)

find_package(Boost COMPONENTS python3 REQUIRED)
find_package(PythonLibs REQUIRED)

add_library(gie STATIC ${LIBRARY_SOURCES})
target_link_libraries(gie PUBLIC ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
target_include_directories(gie PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_include_directories(gie PUBLIC ${Boost_INCLUDE_DIR} ${PYTHON_INCLUDE_DIRS})

install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/)
install(
        TARGETS gie
        EXPORT gie-config
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(
        EXPORT gie-config
        NAMESPACE gie::
        DESTINATION lib/cmake/gie)

# TESTS
add_executable(gie_test tests/test.cpp)
target_link_libraries(gie_test gie Catch2)

file(INSTALL tests/test_modules DESTINATION ./)

add_executable(gie_dev_test tests/dev_test.cpp)
target_link_libraries(gie_dev_test gie)