//
// Created by alex on 4/27/19.
//

#include <gie/Node.h>

std::optional<Node> makeNode(NodeTypeManager& typeManager, std::string name, Arguments arguments)
{
    auto type = typeManager.nodeType(name);

    if(type == nullptr)
        return std::nullopt;

    arguments.resize(type->m_arguments.size(), NoArgument{});

    return Node
            {
                    {std::move(arguments)},
                    *typeManager.getId(name),
                    type->m_function
            };
}