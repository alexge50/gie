//
// Created by alex on 5/23/19.
//

#ifndef GUI_COLORDATA_H
#define GUI_COLORDATA_H

#include <boost/python.hpp>
#include "TypeData.h"
#include <types/Color.h>

class ColorData: public TypeData
{
public:
    explicit ColorData() = default;
    ColorData(Color data): m_data{boost::python::object(data)} {}
    ColorData(const Value& data): m_data{data} {}

    ColorData(const ColorData&) = default;
    ColorData(ColorData&&) = default;

    QtNodes::NodeDataType type() const override
    {
        return {"Color", "Color"};
    }

    const Color& color() const
    {
        return boost::python::extract<Color>(m_data.m_object);
    }

    Value value() override { return m_data; }

private:
    Value m_data;
};


#endif //GUI_COLORDATA_H
