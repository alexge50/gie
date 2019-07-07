//
// Created by alex on 12/1/18.
//

#ifndef GIE_LIBRARY_ARGUMENT_H
#define GIE_LIBRARY_ARGUMENT_H

#include <gie/Type.h>
#include <gie/Value.h>

#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <variant>


struct ArgumentMetadata
{
    std::string m_argumentName;
    Type m_argumentType;
};

struct NoArgument {};

using NodeId = std::size_t;
using ArgumentValue = std::variant<NodeId, Value, NoArgument>;

using Arguments = std::vector<ArgumentValue>;

#endif //GIE_LIBRARY_ARGUMENT_H
