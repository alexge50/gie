//
// Created by alex on 5/19/19.
//

#ifndef GUI_TYPEDATA_H
#define GUI_TYPEDATA_H

#include <gie/Value.h>
#include <gie/Argument.h>
#include <boost/python.hpp>
#undef B0

#include "StringData.h"
#include "NumberData.h"
#include "ColorData.h"

#include <memory>

Value extractGieValue(const std::shared_ptr<QtNodes::NodeData>& nodeData)
{
    if(!nodeData) return Value{};

    const auto& type = nodeData->type().id;

    if(type == "double")
        return Value{boost::python::object(reinterpret_cast<const NumberData*>(nodeData.get())->number())};

    if(type == "string")
        return Value{boost::python::object(reinterpret_cast<const StringData*>(nodeData.get())->string())};

    if(type == "Color")
        return Value{boost::python::object(reinterpret_cast<const ColorData*>(nodeData.get())->color())};

    return Value{};
}

std::shared_ptr<QtNodes::NodeData> extractNodeData(const Value& value)
{
    if(auto x = boost::python::extract<double>{value.m_object}; x.check())
        return std::make_shared<NumberData>(x());

    if(auto x = boost::python::extract<std::string>{value.m_object}; x.check())
        return std::make_shared<StringData>(x());

    if(auto x = boost::python::extract<Color>{value.m_object}; x.check())
        return std::make_shared<ColorData>(x());

    return nullptr;
}

QtNodes::NodeDataType getTypeData(const Type& type)
{
    static std::unordered_map<std::string, QString> typeMap = {{"str", "string"}, {"float", "double"}};

    return {typeMap[type.name()], typeMap[type.name()]};
}

#endif //GUI_TYPEDATA_H
