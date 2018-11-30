//
// Created by alex on 11/17/18.
//

#ifndef GIE_LIBRARY_NODE_H
#define GIE_LIBRARY_NODE_H

#include "DrawableNode.h"
#include "LogicNode.h"

#include <vector>
#include <memory>

struct Node
{
    std::weak_ptr<DrawableNode> m_drawable;
    std::weak_ptr<LogicNode> m_logic;

    std::vector<std::weak_ptr<Node>> m_referencesTo;
};


#endif //GIE_LIBRARY_NODE_H
