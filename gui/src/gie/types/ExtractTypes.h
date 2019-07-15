//
// Created by alex on 5/28/19.
//

#ifndef GUI_EXTRACTTYPES_H
#define GUI_EXTRACTTYPES_H

#include <gie/Value.h>
#include <gie/Argument.h>
#include <boost/python.hpp>

#include "TypeData.h"

#include <memory>

QtNodes::NodeDataType getTypeData(const Type& type)
{
    static std::unordered_map<std::string, QString> typeMap = {{"str", "string"}, {"float", "double"}, {"int", "integer"}, {"Color", "Color"}, {"Image", "Image"}};

    return {typeMap[type.name()], typeMap[type.name()]};
}

#endif //GUI_EXTRACTTYPES_H
