//
// Created by alex on 11/17/18.
//

#ifndef GIE_LIBRARY_NODE_H
#define GIE_LIBRARY_NODE_H

#include <gie/Argument.h>
#include <gie/NodeType.h>
#include <gie/NodeTypeManager.h>

#include <vector>
#include <memory>
#include <cstddef>

using NodeId = std::size_t;

struct Node
{
    Arguments arguments;
    NodeTypeId nodeTypeId;
};

std::optional<Node> makeNode(NodeTypeManager&, std::string name, Arguments);

#endif //GIE_LIBRARY_NODE_H
