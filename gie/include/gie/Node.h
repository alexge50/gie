#include <utility>

#include <utility>

//
// Created by alex on 11/17/18.
//

#ifndef GIE_LIBRARY_NODE_H
#define GIE_LIBRARY_NODE_H

#include <gie/Argument.h>
#include <gie/NodeId.h>
#include <gie/NodeType.h>
#include <gie/NodeTypeManager.h>

#include <vector>
#include <memory>
#include <cstddef>

class Node
{
private:
    Node(Arguments arguments, NodeTypeId nodeTypeId, boost::python::object function):
        arguments{std::move(arguments)},
        m_nodeTypeId{nodeTypeId},
        m_function{std::move(function)}
    {}

public:
    Arguments arguments;

public:
    const NodeTypeId& nodeTypeId() const { return m_nodeTypeId; }
    const boost::python::object& function() const { return m_function; }

private:
    NodeTypeId m_nodeTypeId;
    boost::python::object m_function;

    friend std::optional<Node> makeNode(NodeTypeManager&, std::string name, Arguments);
};

std::optional<Node> makeNode(NodeTypeManager&, std::string name, Arguments);

#endif //GIE_LIBRARY_NODE_H
