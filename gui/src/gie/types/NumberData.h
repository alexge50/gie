//
// Created by alex on 5/19/19.
//

#ifndef GUI_NUMBERDATA_H
#define GUI_NUMBERDATA_H

#include <boost/python.hpp>
#include "TypeData.h"

class NumberData: public TypeData
{
public:
    explicit NumberData() = default;
    NumberData(double data): m_data{boost::python::object(data)} {}
    NumberData(const Value& data): m_data{data} {}

    NumberData(const NumberData&) = default;
    NumberData(NumberData&&) = default;

    QtNodes::NodeDataType type() const override
    {
        return {"double", "double"};
    }

    double number() const
    {
        return  boost::python::extract<double>(m_data.m_object);
    }

    QString asText() const
    {
        return QString::number(boost::python::extract<double>(m_data.m_object));
    }

    Value value() override { return m_data; }

private:
    Value m_data;
};

#endif //GUI_NUMBERDATA_H
