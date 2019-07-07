//
// Created by alex on 11/17/18.
//

#ifndef GIE_LIBRARY_NODE_H
#define GIE_LIBRARY_NODE_H

#include <gie/NodeDrawable.h>
#include <gie/NodeLogic.h>
#include <gie/NodeType.h>
#include <gie/NodeTypeManager.h>

#include <vector>
#include <memory>
#include <cstddef>

using NodeId = std::size_t;

struct Node
{
    NodeDrawable m_drawable;
    NodeLogic m_logic;
    NodeTypeId m_metadata;
};

std::optional<Node> makeNode(NodeTypeManager&, std::string name, std::vector<ArgumentValue>);

#endif //GIE_LIBRARY_NODE_H
