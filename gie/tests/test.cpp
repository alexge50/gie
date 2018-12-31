//
// Created by alex on 11/17/18.
//

#include <gie/Node.h>
#include <gie/Graph.h>
#include <gie/Program.h>

#include <boost/python.hpp>

#include <cassert>
#include <vector>

int main()
{
    Program program;

    boost::python::object input{10};
    NodeId castToString = program.addNode(Node{{}, {"str", std::vector<std::pair<Argument, std::variant<NodeId, Value>>>{std::make_pair(Argument{"input", {"int"}}, std::variant<NodeId, Value>{Value{"int", input}})}}});

    auto result = program.run();

    assert(boost::python::extract<int>(input) == boost::python::extract<int>(result.value().m_object));

    return 0;
}