//
// Created by alex on 4/26/19.
//

#ifndef GIE_LIBRARY_NODEMETADATA_H
#define GIE_LIBRARY_NODEMETADATA_H

#include <gie/Argument.h>
#include <gie/Type.h>
#include <vector>
#include <string>

struct NodeMetadata
{
    std::string m_functionName;
    std::vector<ArgumentMetadata> m_arguments;
    Type m_returnType;
};

#endif //GIE_LIBRARY_NODEMETADATA_H
