//
// Created by alex on 5/28/19.
//

#ifndef GUI_EXTRACTTYPES_H
#define GUI_EXTRACTTYPES_H

#include <gie/Value.h>
#include <gie/Argument.h>
#include <boost/python.hpp>

#include "StringData.h"
#include "NumberData.h"
#include "IntegerData.h"
#include "ColorData.h"
#include "ImageData.h"

#include "TypeData.h"

#include <memory>

inline Value extractGieValue(const std::shared_ptr<QtNodes::NodeData>& nodeData)
{
    if(auto p = dynamic_cast<TypeData*>(nodeData.get()); p)
    {
        return p->value();
    }

    return Value{};
}

inline std::shared_ptr<QtNodes::NodeData> extractNodeData(const Value& value)
{
    if(auto x = boost::python::extract<long long>{value.m_object}; x.check() && PyLong_Check(value.m_object.ptr()))
        return std::make_shared<IntegerData>(value);

    if(auto x = boost::python::extract<double>{value.m_object}; x.check() && PyFloat_Check(value.m_object.ptr()))
        return std::make_shared<NumberData>(value);

    if(auto x = boost::python::extract<std::string>{value.m_object}; x.check())
        return std::make_shared<StringData>(value);

    if(auto x = boost::python::extract<Color>{value.m_object}; x.check())
        return std::make_shared<ColorData>(value);

    if(auto x = boost::python::extract<Image>{value.m_object}; x.check())
        return std::make_shared<ImageData>(value);

    return nullptr;
}

inline std::shared_ptr<QtNodes::NodeData> extractNodeData(const Type& type, const Value& value)
{
    if(type == Type("str"))
        return std::make_shared<StringData>(value);

    if(type == Type("float"))
        return std::make_shared<NumberData>(value);

    if(type == Type("int"))
        return std::make_shared<IntegerData>(value);

    if(type == Type("Color"))
        return std::make_shared<ColorData>(value);

    if(type == Type("Image"))
        return std::make_shared<ImageData>(value);

    return nullptr;
}

inline QtNodes::NodeDataType getTypeData(const Type& type)
{
    static std::unordered_map<std::string, QString> typeMap = {{"str", "string"}, {"float", "double"}, {"int", "integer"}, {"Color", "Color"}, {"Image", "Image"}};

    return {typeMap[type.name()], typeMap[type.name()]};
}

#endif //GUI_EXTRACTTYPES_H
