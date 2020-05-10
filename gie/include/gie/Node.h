//
// Created by alex on 11/17/18.
//

#ifndef GIE_LIBRARY_NODE_H
#define GIE_LIBRARY_NODE_H

#include <gie/Argument.h>
#include <gie/NodeId.h>
#include <gie/PythonContext.h>

#include <vector>
#include <memory>
#include <cstddef>
#include <utility>

struct Node
{
    NodeId nodeId;
    SymbolId symbolId;
    std::vector<std::variant<ResourceId, NodeId>> arguments;
};

#endif //GIE_LIBRARY_NODE_H
