//
// Created by alex on 5/19/19.
//

#ifndef GUI_TYPEDATA_H
#define GUI_TYPEDATA_H

#include <gie/NodeId.h>
#include <nodes/NodeData>

#include <types/Image.h>
#include <types/Color.h>

#include <variant>
#include <string>

#include <src/GieNodeId.h>

class TypeData: public QtNodes::NodeData
{
public:
    explicit TypeData() = default;
    explicit TypeData(GieNodeId id): m_id{id} {}
    explicit TypeData(QUuid id): m_id{id} {}

    TypeData(const TypeData&) = default;
    TypeData(TypeData&&) = default;

    const auto& nodeId() { return std::get<GieNodeId>(m_id); }
    const auto& valueId() { return std::get<QUuid>(m_id); }

    bool isValueId() { return std::holds_alternative<QUuid>(m_id); }
    bool isNodeId() { return std::holds_alternative<GieNodeId>(m_id); }

private:
    std::variant<GieNodeId, QUuid> m_id;
};

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
