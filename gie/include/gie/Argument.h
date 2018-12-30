//
// Created by alex on 12/1/18.
//

#ifndef GIE_LIBRARY_ARGUMENT_H
#define GIE_LIBRARY_ARGUMENT_H

#include <string>

struct ArgumentType
{
    std::string m_typename;
};

struct Argument
{
    std::string m_argumentName;
    ArgumentType m_argumentType;
};


#endif //GIE_LIBRARY_ARGUMENT_H
