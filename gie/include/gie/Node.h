//
// Created by alex on 11/17/18.
//

#ifndef GIE_LIBRARY_NODE_H
#define GIE_LIBRARY_NODE_H

#include "DrawableNode.h"
#include "LogicNode.h"

#include <vector>
#include <memory>
#include <cstddef>

using NodeId = std::size_t;

struct Node
{
    DrawableNode m_drawable;
    LogicNode m_logic;
};


#endif //GIE_LIBRARY_NODE_H
