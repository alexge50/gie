//
// Created by alex on 11/17/18.
//
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <gie/Node.h>
#include <gie/Program.h>
#include <gie/NodeUtil.h>

#include <boost/python.hpp>

#include <cassert>
#include <vector>

#include <iostream>


TEST_CASE("GIE API tests", "[program]")
{

    Program program;
    program.import("builtins");

    boost::python::object input(10);

    SECTION("a couple of nodes")
    {
        NodeId castToString, castToInt;
        castToString = program.addNode(makeNode(program.context(), "str", {ArgumentValue{Value{input}}}));
        SECTION("1 node run")
        {
            auto result = program.run();
            REQUIRE(std::to_string(boost::python::extract<int>(input)) == std::string{boost::python::extract<std::string>(result.value().m_object)});
        }

        castToInt = program.addNode(makeNode(program.context(), "int", {castToString}));

        SECTION("2 nodes run")
        {
            auto result = program.run();
            REQUIRE(boost::python::extract<int>(input) == boost::python::extract<int>(result.value().m_object));
        }

        SECTION("removing node")
        {
            program.removeNode(castToInt);
            auto result = program.run();
            REQUIRE(std::to_string(boost::python::extract<int>(input)) == std::string{boost::python::extract<std::string>(result.value().m_object)});

            program.removeNode(castToString);
            auto result2 = program.run();
            REQUIRE(!result2.has_value());
        }
    }

    SECTION("many nodes")
    {
        std::vector<NodeId> castToString;
        std::vector<NodeId> castToInt;

        castToString.push_back(program.addNode(makeNode(program.context(), "str", {ArgumentValue{Value{input}}})));
        castToInt.push_back(program.addNode(makeNode(program.context(), "int", {castToString.back()})));
        for(int i = 0; i < 100; i ++)
        {
            castToString.push_back(program.addNode(makeNode(program.context(), "str", {castToInt.back()})));
            castToInt.push_back(program.addNode(makeNode(program.context(), "int", {castToString.back()})));
        }

        SECTION("run")
        {
            auto result = program.run();
            REQUIRE(boost::python::extract<int>(input) == boost::python::extract<int>(result.value().m_object));
        }

        SECTION("removing nodes in the middle")
        {
            auto stringId = castToString[50];
            auto intId = castToInt[50];

            program.removeNode(stringId);
            program.removeNode(intId);

            program.editNode(castToString[51], makeNode(program.context(), "str", {castToInt[49]}));

            auto result = program.run();
            REQUIRE(std::to_string(boost::python::extract<int>(input)) == std::string{boost::python::extract<std::string>(result.value().m_object)});
        }

        SECTION("removing all nodes")
        {
            for(auto i: castToString)
                program.removeNode(i);
            for(auto i: castToInt)
                program.removeNode(i);

            auto result = program.run();
            REQUIRE(!result.has_value());
        }
    }

}
