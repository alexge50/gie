//
// Created by alex on 4/27/19.
//

#include <gie/NodeUtil.h>

#include <iostream>

static bool hasattr(boost::python::object object, const char* name)//TODO: move to python utils
{
    return static_cast<bool>(PyObject_HasAttrString(object.ptr(), name));
}

static std::vector<ArgumentMetadata> fetchArguments(PythonContext& context, boost::python::object callable)
{
    using namespace boost::python;

    auto inspect = context.module("inspect", false);
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

static Type fetchReturnType(PythonContext& context, boost::python::object callable)
{
    using namespace boost::python;

    auto inspect = context.module("inspect", false);
    auto signature = inspect.attr("signature")(callable);

    if(hasattr(signature, "return_annotation"))
        return Type{extract<std::string>(signature.attr("return_annotation").attr("__name__"))};
    else return Type{""};
}

NodeMetadata fetchMetadata(PythonContext& context, std::string qualifiedFunctionName)
{
    auto object = context.getFunction(qualifiedFunctionName);

    return NodeMetadata
    {
        object,
        fetchArguments(context, object),
        fetchReturnType(context, object),
        createSymbol(qualifiedFunctionName)
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