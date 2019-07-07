//
// Created by alex on 7/7/19.
//

#ifndef GIE_NODETYPE_H
#define GIE_NODETYPE_H

#include <gie/Argument.h>
#include <gie/Type.h>
#include <gie/PythonContext.h>

#include <vector>
#include <string>
#include <boost/python.hpp>

struct NodeType
{
    boost::python::object m_function;
    std::vector<ArgumentMetadata> m_arguments;
    Type m_returnType;
    Symbol m_symbol;
};

#endif //GIE_NODETYPE_H
