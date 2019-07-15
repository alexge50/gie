//
// Created by alex on 5/19/19.
//

#ifndef GUI_TYPEDATA_H
#define GUI_TYPEDATA_H

#include <gie/NodeId.h>
#include <nodes/NodeData>

class TypeData: public QtNodes::NodeData
{
public:
    explicit TypeData(): m_nodeId{std::nullopt} {}
    explicit TypeData(NodeId id): m_nodeId{id} {}

    TypeData(const TypeData&) = default;
    TypeData(TypeData&&) = default;

    const auto& nodeId() { return m_nodeId; }

private:
    std::optional<NodeId> m_nodeId;
};

class StringTypeData: public TypeData
{
public:
    QtNodes::NodeDataType type() const override
    {
        return {"string", "string"};
    }
};

class NumberTypeData: public TypeData
{
public:
    QtNodes::NodeDataType type() const override
    {
        return {"number", "number"};
    }
};

class IntegerTypeData: public TypeData
{
public:
    QtNodes::NodeDataType type() const override
    {
        return {"integer", "integer"};
    }
};

class ImageTypeData: public TypeData
{
public:
    QtNodes::NodeDataType type() const override
    {
        return {"Image", "Image"};
    }
};

class ColorTypeData: public TypeData
{
public:
    QtNodes::NodeDataType type() const override
    {
        return {"Color", "Color"};
    }
};


#endif //GUI_TYPEDATA_H
