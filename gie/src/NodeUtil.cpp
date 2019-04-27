//
// Created by alex on 4/27/19.
//

#include <gie/NodeUtil.h>

static std::vector<ArgumentMetadata> fetchArguments(PythonContext& context, const boost::python::object& callable)
{
    using namespace boost::python;

    auto inspect = context.module("inspect");
    auto signature = inspect.attr("signature")(callable);

    std::vector<object> parameters{
        stl_input_iterator<object>(signature.attr("parameters").attr("items")()),
        stl_input_iterator<object>{}
    };

    std::vector<ArgumentMetadata> arguments;
    arguments.reserve(parameters.size());

    for(const auto& parameter: parameters)
    {
        auto p = parameter[1];
        arguments.push_back(ArgumentMetadata{
                extract<std::string>(p.attr("name")),
                Type{extract<std::string>(p.attr("annotation").attr("__name__"))}
        });
    }

    return arguments;
}

static Type fetchReturnType(PythonContext& context, const boost::python::object& callable)
{
    using namespace boost::python;

    auto inspect = context.module("inspect");
    auto signature = inspect.attr("signature")(callable);

    return Type{extract<std::string>(signature.attr("return_annotation").attr("__name__"))};
}

NodeMetadata fetchMetadata(PythonContext& context, std::string name)
{
    auto object = context.getFunction(name);

    return NodeMetadata
    {
        object,
        fetchArguments(context, object),
        fetchReturnType(context, object),
        std::move(name)
    };
}

Node makeNode(PythonContext& context, std::string name, std::vector<ArgumentValue> arguments)
{
    NodeMetadata metadata = fetchMetadata(context, name);

    //TODO: Check parameters

    return Node
    {
        {},
        {std::move(arguments)},
        metadata
    };
}