//
// Created by alex on 11/30/18.
//

#ifndef GIE_LIBRARY_LOGICNODE_H
#define GIE_LIBRARY_LOGICNODE_H

#include "Argument.h"
#include "Value.h"

#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <variant>

using NodeId = std::size_t;

struct LogicNode
{
    std::string m_functionName;
    std::vector<std::pair<Argument, std::variant<NodeId, Value>>> m_argument;
};


#endif //GIE_LIBRARY_LOGICNODE_H
