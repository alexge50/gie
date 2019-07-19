#include <utility>

#include <utility>

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

class Node
{
private:
    Node(Arguments arguments, SymbolId symbolId):
        arguments{std::move(arguments)},
        m_symbolId{symbolId}
    {}

public:
    Arguments arguments;

public:
    const SymbolId& symbolId() const { return m_symbolId; }

private:
    SymbolId m_symbolId;

    friend std::optional<Node> makeNode(const PythonContext&, const std::string& name, Arguments);
};

std::optional<Node> makeNode(const PythonContext&, const std::string& name, Arguments);

#endif //GIE_LIBRARY_NODE_H
