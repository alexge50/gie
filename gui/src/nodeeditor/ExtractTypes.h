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

inline QtNodes::NodeDataType getTypeData(const Type& type)
{
    static std::unordered_map<std::string, QString> typeMap = {{"str", "string"}, {"float", "double"}, {"int", "integer"}, {"Color", "Color"}, {"Image", "Image"}};

    return {typeMap[type.name()], typeMap[type.name()]};
}

inline std::shared_ptr<QtNodes::NodeData> makeTypeData(const Type& type)
{
    using Callback = std::shared_ptr<QtNodes::NodeData>(*)();
    static std::unordered_map<std::string, Callback> typeMap = {
            {"str", [](){
                return std::shared_ptr<QtNodes::NodeData>(std::make_shared<StringTypeData>());
            }},
            {"float", [](){
                return std::shared_ptr<QtNodes::NodeData>(std::make_shared<NumberTypeData>());
            }},
            {"int", [](){
                return std::shared_ptr<QtNodes::NodeData>(std::make_shared<IntegerTypeData>());
            }},
            {"Color", [](){
                return std::shared_ptr<QtNodes::NodeData>(std::make_shared<ColorTypeData>());
            }},
            {"Image", [](){
                return std::shared_ptr<QtNodes::NodeData>(std::make_shared<ImageTypeData>());
            }}
    };

    return typeMap[type.name()]();
}

#endif //GUI_EXTRACTTYPES_H
