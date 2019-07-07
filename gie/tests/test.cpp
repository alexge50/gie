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
    program.context().module("builtins", false);

    auto sys = program.context().module("sys", false);
    auto os = program.context().module("os", false);

    sys.attr("path").attr("insert")(1, os.attr("getcwd")());

    program.import("basic", "test_modules/basic.py");

    boost::python::object input(10);

    SECTION("a couple of nodes")
    {
        NodeId castToString, castToInt;
        castToString = program.addNode(makeNode(program.context(), "basic.to_string", {ArgumentValue{Value{input}}}));
        SECTION("1 node run") {
            program.addResult("1 node run", castToString);
            auto result = program.run();
            REQUIRE(std::to_string(boost::python::extract<int>(input)) ==
                    std::string{boost::python::extract<std::string>(result[0].value.m_object)});
        }

        castToInt = program.addNode(makeNode(program.context(), "basic.to_int", {castToString}));

        SECTION("2 nodes run")
        {
            program.addResult("1 nodes run", castToInt);
            auto result = program.run();
            REQUIRE(boost::python::extract<int>(input) == boost::python::extract<int>(result[0].value.m_object));
        }

        SECTION("removing node")
        {
            program.removeNode(castToInt);
            program.addResult("toString", castToString);
            auto result = program.run();
            REQUIRE(std::to_string(boost::python::extract<int>(input)) ==
                    std::string{boost::python::extract<std::string>(result[0].value.m_object)});

            program.removeResult("toString");
            program.removeNode(castToString);
            auto result2 = program.run();
            REQUIRE(result2.size() == 0);
        }
    }


    SECTION("many nodes")
    {
        std::vector<NodeId> castToString;
        std::vector<NodeId> castToInt;

        castToString.push_back(
                program.addNode(makeNode(program.context(), "basic.to_string", {ArgumentValue{Value{input}}})));
        castToInt.push_back(program.addNode(makeNode(program.context(), "basic.to_int", {castToString.back()})));
        for (int i = 0; i < 100; i++)
        {
            castToString.push_back(program.addNode(makeNode(program.context(), "basic.to_string", {castToInt.back()})));
            castToInt.push_back(program.addNode(makeNode(program.context(), "basic.to_int", {castToString.back()})));
        }

        program.addResult("result", castToInt.back());

        SECTION("run")
        {
            auto result = program.run();
            REQUIRE(boost::python::extract<int>(input) == boost::python::extract<int>(result[0].value.m_object));
        }

        SECTION("removing nodes in the middle")
        {
            auto stringId = castToString[50];
            auto intId = castToInt[50];

            program.removeNode(stringId);
            program.removeNode(intId);

            program.editNode(castToString[51], makeNode(program.context(), "basic.to_string", {castToInt[49]}));

            auto result = program.run();
            REQUIRE(std::to_string(boost::python::extract<int>(input)) ==
                    std::to_string(boost::python::extract<int>(result[0].value.m_object)));
        }

        SECTION("removing all nodes")
        {
            program.removeResult("result");

            for (auto i: castToString)
                program.removeNode(i);
            for (auto i: castToInt)
                program.removeNode(i);

            auto result = program.run();
            REQUIRE(result.size() == 0);
        }
    }
}
