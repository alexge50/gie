//
// Created by alex on 12/1/18.
//

#ifndef GIE_LIBRARY_SCENEGRAPH_H
#define GIE_LIBRARY_SCENEGRAPH_H

#include <gie/Node.h>
#include <gie/Error.h>

#include <MaybeError.h>

#include <optional>

struct ScriptGraph
{
    std::vector<Node> nodes;
    std::unordered_map<NodeId, Value> nodesData;
    std::unordered_map<ResourceId, Value> resources;
};

#endif //GIE_LIBRARY_SCENEGRAPH_H
