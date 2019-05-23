//
// Created by alex on 5/23/19.
//

#ifndef GUI_COLORDATA_H
#define GUI_COLORDATA_H

#include <nodes/NodeData>
#include <modules/Color.h>

class ColorData: public QtNodes::NodeData
{
public:
    explicit ColorData() = default;
    ColorData(Color data): m_data{data} {}

    ColorData(const ColorData&) = default;
    ColorData(ColorData&&) = default;

    QtNodes::NodeDataType type() const override
    {
        return {"Color", "Color"};
    }

    const Color& color() const
    {
        return m_data;
    }

private:
    Color m_data;
};


#endif //GUI_COLORDATA_H
