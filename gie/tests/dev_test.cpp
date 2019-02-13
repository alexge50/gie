//
// Created by alex on 2/2/19.
//

#include <gie/Node.h>
#include <gie/Program.h>

#include <boost/python.hpp>

#include <cassert>
#include <vector>

#include <iostream>

int main()
{

    Program program;
    NodeId castToString, castToInt;

    program.import("builtins");

    boost::python::object input(10);

    {
        castToString = program.addNode(Node{{}, {"str", std::vector<std::pair<Argument, std::variant<NodeId, Value>>>{std::make_pair(Argument{"", {"int"}}, std::variant<NodeId, Value>{Value{"int", input}})}}});

        auto result = program.run();

        std::cout << (std::to_string(boost::python::extract<int>(input)) == std::string{boost::python::extract<std::string>(result.value().m_object)}) << std::endl;
    }


    {
        castToInt = program.addNode(Node{{}, {"int", std::vector<std::pair<Argument, std::variant<NodeId, Value>>>{std::make_pair(Argument{"", {"str"}}, std::variant<NodeId, Value>{castToString})}}});

        auto result = program.run();

        std::cout << (boost::python::extract<int>(input) == boost::python::extract<int>(result.value().m_object)) << std::endl;
    }

    {
        program.removeNode(castToInt);
        auto result = program.run();
        std::cout << (std::to_string(boost::python::extract<int>(input)) == std::string{boost::python::extract<std::string>(result.value().m_object)});

        program.removeNode(castToString);
        auto result2 = program.run();
    }
}
