//
// Created by alex on 5/24/19.
//

#ifndef GUI_INTDATA_H
#define GUI_INTDATA_H

#include <boost/python.hpp>
#include "TypeData.h"

class IntegerData: public TypeData
{
public:
    explicit IntegerData() = default;
    IntegerData(long long int data): m_data{boost::python::object(data)} {}
    IntegerData(const Value& data): m_data{data} {}

    IntegerData(const IntegerData&) = default;
    IntegerData(IntegerData&&) = default;

    QtNodes::NodeDataType type() const override
    {
        return {"integer", "integer"};
    }

    long long int number() const
    {
        return boost::python::extract<long long int>(m_data.m_object);
    }

    QString asText() const
    {
        return QString::number(boost::python::extract<long long int>(m_data.m_object));
    }

    Value value() override { return m_data; }

private:
    Value m_data;
};

#endif //GUI_INTDATA_H
