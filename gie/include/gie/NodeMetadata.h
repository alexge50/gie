//
// Created by alex on 4/26/19.
//

#ifndef GIE_LIBRARY_NODEMETADATA_H
#define GIE_LIBRARY_NODEMETADATA_H

#include <gie/Argument.h>
#include <gie/Type.h>

#include <vector>
#include <string>
#include <boost/python.hpp>

struct NodeMetadata
{
    boost::python::object m_function;
    std::vector<ArgumentMetadata> m_arguments;
    Type m_returnType;
    std::string m_functionName;
};

#endif //GIE_LIBRARY_NODEMETADATA_H
