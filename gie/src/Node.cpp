//
// Created by alex on 4/27/19.
//

#include <gie/Node.h>

std::optional<Node> makeNode(const PythonContext& context, const std::string& name, Arguments arguments)
{
    auto symbolId = context.getSymbolId(name);

    if(!symbolId)
        return std::nullopt;

    arguments.resize(context.getSymbol(*symbolId)->arguments.size(), NoArgument{});

    return Node
            {
                    {std::move(arguments)},
                    *symbolId
            };
}