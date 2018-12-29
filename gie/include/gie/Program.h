//
// Created by alex on 12/3/18.
//

#ifndef GIE_LIBRARY_PROGRAM_H
#define GIE_LIBRARY_PROGRAM_H

#include "Graph.h"

#include <optional>

class Program
{
public:
    Program();

    void run();
    std::optional<Value> getResult();

    NodeId addNode(const Node &node);
    void editNode(NodeId, const Node &node);
    void removeNode(NodeId);

private:
    Graph m_graph;
};


#endif //GIE_LIBRARY_PROGRAM_H
