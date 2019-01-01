//
// Created by alex on 11/17/18.
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

    program.import("builtins");

    boost::python::object input(10);
    NodeId castToString = program.addNode(Node{{}, {"str", std::vector<std::pair<Argument, std::variant<NodeId, Value>>>{std::make_pair(Argument{"", {"int"}}, std::variant<NodeId, Value>{Value{"int", input}})}}});

    auto result = program.run();

    std::cout << boost::python::extract<int>(input) << std::endl;
    std::cout << std::string{boost::python::extract<std::string>(result.value().m_object)} << std::endl;

    assert(std::to_string(boost::python::extract<int>(input)) == std::string{boost::python::extract<std::string>(result.value().m_object)});

    return 0;
}