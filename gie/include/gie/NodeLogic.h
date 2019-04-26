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
using ArgumentValue = std::variant<NodeId, Value>;

struct NodeLogic
{
    std::vector<ArgumentValue> m_argument;
};

#endif //GIE_LIBRARY_LOGICNODE_H
