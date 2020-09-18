#ifndef NODE_EDITOR_RENDERSTATE_H
#define NODE_EDITOR_RENDERSTATE_H

#include <Graph.h>

#include <unordered_map>


struct RenderState
{
    std::unordered_map<NodeId, float> node_position_z;
};

#endif //NODE_EDITOR_RENDERSTATE_H
