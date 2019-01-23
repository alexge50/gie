//
// Created by alex on 12/1/18.
//

#ifndef GIE_LIBRARY_SCENEGRAPH_H
#define GIE_LIBRARY_SCENEGRAPH_H

#include "Node.h"
#include "util/Graph.h"

#include <utility>

using SceneGraph = util::Graph<std::pair<Node, Value>>;

#endif //GIE_LIBRARY_SCENEGRAPH_H
