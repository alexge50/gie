//
// Created by alex on 11/17/18.
//

#ifndef GIE_LIBRARY_NODE_H
#define GIE_LIBRARY_NODE_H

#include "NodeDrawable.h"
#include "NodeLogic.h"
#include "NodeMetadata.h"

#include <vector>
#include <memory>
#include <cstddef>

using NodeId = std::size_t;

struct Node
{
    NodeDrawable m_drawable;
    NodeLogic m_logic;
    NodeMetadata m_metadata;
};


#endif //GIE_LIBRARY_NODE_H
