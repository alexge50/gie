//
// Created by alex on 11/17/18.
//
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <gie/Node.h>
#include <gie/Program.h>

#include <boost/python.hpp>

#include <cassert>
#include <vector>

#include <iostream>


TEST_CASE("GIE API tests", "[program]")
{

    SECTION("adding nodes")
    {

        Program program;
        NodeId castToString, castToInt;

        program.import("builtins");

        boost::python::object input(10);

        {
            castToString = program.addNode(Node{{}, {"str", std::vector<std::pair<Argument, std::variant<NodeId, Value>>>{std::make_pair(Argument{"", {"int"}}, std::variant<NodeId, Value>{Value{"int", input}})}}});

            auto result = program.run();

            REQUIRE(std::to_string(boost::python::extract<int>(input)) == std::string{boost::python::extract<std::string>(result.value().m_object)});
        }

        {
            castToInt = program.addNode(Node{{}, {"int", std::vector<std::pair<Argument, std::variant<NodeId, Value>>>{std::make_pair(Argument{"", {"str"}}, std::variant<NodeId, Value>{castToString})}}});

            auto result = program.run();

            REQUIRE(boost::python::extract<int>(input) == boost::python::extract<int>(result.value().m_object));
        }
    }

}