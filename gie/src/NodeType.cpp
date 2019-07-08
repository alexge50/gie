//
// Created by alex on 7/7/19.
//

#include <gie/NodeType.h>

static bool hasattr(boost::python::object object, const char* name)//TODO: move to python utils
{
    return PyObject_HasAttrString(object.ptr(), name) != 0;
}

static std::vector<ArgumentMetadata> fetchArguments(const PythonContext& context, boost::python::object callable)
{
    using namespace boost::python;

    auto inspect = context.inspect();
    auto signature = inspect.attr("signature")(callable);

    std::vector<object> parameters{
            stl_input_iterator<object>(signature.attr("parameters").attr("items")()),
            stl_input_iterator<object>{}
    };

    std::vector<ArgumentMetadata> arguments{};
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

static Type fetchReturnType(const PythonContext& context, boost::python::object callable)
{
    using namespace boost::python;

    auto inspect = context.inspect();
    auto signature = inspect.attr("signature")(callable);

    if(hasattr(signature, "return_annotation"))
        return Type{extract<std::string>(signature.attr("return_annotation").attr("__name__"))};
    else return Type{""};
}

NodeType fetchNodeType(const PythonContext& context, std::string qualifiedFunctionName)
{
    auto object = context.getFunction(qualifiedFunctionName);

    return NodeType
            {
                    object,
                    fetchArguments(context, object),
                    fetchReturnType(context, object),
                    createSymbol(qualifiedFunctionName)
            };
}
