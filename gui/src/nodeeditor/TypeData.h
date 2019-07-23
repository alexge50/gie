//
// Created by alex on 5/19/19.
//

#ifndef GUI_TYPEDATA_H
#define GUI_TYPEDATA_H

#include <nodes/NodeData>

#include <types/Image.h>
#include <types/Color.h>

#include <variant>
#include <string>

class TypeData: public QtNodes::NodeData
{};

class StringTypeData: public TypeData
{
public:
    using TypeData::TypeData;

    QtNodes::NodeDataType type() const override
    {
        return {"string", "string"};
    }
};

class NumberTypeData: public TypeData
{
public:
    using TypeData::TypeData;

    QtNodes::NodeDataType type() const override
    {
        return {"double", "double"};
    }
};

class IntegerTypeData: public TypeData
{
public:
    using TypeData::TypeData;

    QtNodes::NodeDataType type() const override
    {
        return {"integer", "integer"};
    }
};

class ImageTypeData: public TypeData
{
public:
    using TypeData::TypeData;

    QtNodes::NodeDataType type() const override
    {
        return {"Image", "Image"};
    }
};

class ColorTypeData: public TypeData
{
public:
    using TypeData::TypeData;

    QtNodes::NodeDataType type() const override
    {
        return {"Color", "Color"};
    }
};

using Data = std::variant<double, long long int, Color, Image, std::string>;


#endif //GUI_TYPEDATA_H
