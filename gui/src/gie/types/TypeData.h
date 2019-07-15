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

class StringData: public TypeData
{
public:
    QtNodes::NodeDataType type() const override
    {
        return {"string", "string"};
    }
};

class NumberData: public TypeData
{
public:
    QtNodes::NodeDataType type() const override
    {
        return {"number", "number"};
    }
};

class IntegerData: public TypeData
{
public:
    QtNodes::NodeDataType type() const override
    {
        return {"integer", "integer"};
    }
};

class ImageData: public TypeData
{
public:
    QtNodes::NodeDataType type() const override
    {
        return {"image", "image"};
    }
};

class ColorData: public TypeData
{
public:
    QtNodes::NodeDataType type() const override
    {
        return {"color", "color"};
    }
};


#endif //GUI_TYPEDATA_H
