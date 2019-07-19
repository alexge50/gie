//
// Created by alex on 11/17/18.
//
#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include <gie/Program.h>

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
        castToString = program.addNode("basic.to_string", {ArgumentValue{Value{input}}});
        SECTION("1 node run") {
            program.addResult("1 node run", castToString);
            auto result = program.run().value();
            REQUIRE(std::to_string(boost::python::extract<int>(input)) ==
                    std::string{boost::python::extract<std::string>(result[0].value.object())});
        }

        castToInt = program.addNode("basic.to_int", {castToString});

        SECTION("2 nodes run")
        {
            program.addResult("1 nodes run", castToInt);
            auto result = program.run().value();
            REQUIRE(boost::python::extract<int>(input) == boost::python::extract<int>(result[0].value.object()));
        }

        SECTION("removing node")
        {
            program.removeNode(castToInt).discard();
            program.addResult("toString", castToString);
            auto result = program.run().value();
            REQUIRE(std::to_string(boost::python::extract<int>(input)) ==
                    std::string{boost::python::extract<std::string>(result[0].value.object())});

            program.removeResult("toString");
            program.removeNode(castToString).discard();
            auto result2 = program.run();
            REQUIRE(result2->size() == 0);
        }
    }


    SECTION("many nodes")
    {
        std::vector<NodeId> castToString;
        std::vector<NodeId> castToInt;

        castToString.push_back(
                program.addNode("basic.to_string", {ArgumentValue{Value{input}}}));
        castToInt.push_back(program.addNode("basic.to_int", {castToString.back()}));
        for (int i = 0; i < 100; i++)
        {
            castToString.push_back(program.addNode("basic.to_string", {castToInt.back()}));
            castToInt.push_back(program.addNode("basic.to_int", {castToString.back()}));
        }

        program.addResult("result", castToInt.back());

        SECTION("run")
        {
            auto result = program.run().value();
            REQUIRE(boost::python::extract<int>(input) == boost::python::extract<int>(result[0].value.object()));
        }

        SECTION("removing nodes in the middle")
        {
            auto stringId = castToString[50];
            auto intId = castToInt[50];

            program.removeNode(stringId).discard();
            program.removeNode(intId).discard();

            program.editNode(castToString[51], ArgumentId{0}, ArgumentValue{castToInt[49]}).discard();

            auto result = program.run().value();
            REQUIRE(std::to_string(boost::python::extract<int>(input)) ==
                    std::to_string(boost::python::extract<int>(result[0].value.object())));
        }

        SECTION("removing all nodes")
        {
            program.removeResult("result");

            for (auto i: castToString)
                program.removeNode(i).discard();
            for (auto i: castToInt)
                program.removeNode(i).discard();

            auto result = program.run();
            REQUIRE(result->size() == 0);
        }
    }
}
