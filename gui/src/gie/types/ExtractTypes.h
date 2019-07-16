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

template <typename T>
std::shared_ptr<QtNodes::NodeData> makeTypeData(const Type& type, T&& t)
{
    using Callback = std::shared_ptr<QtNodes::NodeData>(*)(T&&);
    static std::unordered_map<std::string, Callback> typeMap = {
            {"str", [](const T& t){
                return std::shared_ptr<QtNodes::NodeData>(std::make_shared<StringTypeData>(t));
            }},
            {"float", [](const T& t){
                return std::shared_ptr<QtNodes::NodeData>(std::make_shared<NumberTypeData>(t));
            }},
            {"int", [](const T& t){
                return std::shared_ptr<QtNodes::NodeData>(std::make_shared<IntegerTypeData>(t));
            }},
            {"Color", [](const T& t){
                return std::shared_ptr<QtNodes::NodeData>(std::make_shared<ColorTypeData>(t));
            }},
            {"Image", [](const T& t){
                return std::shared_ptr<QtNodes::NodeData>(std::make_shared<ImageTypeData>(t));
            }}
    };

    return typeMap[type.name()](std::forward<T>(t));
}

#endif //GUI_EXTRACTTYPES_H
