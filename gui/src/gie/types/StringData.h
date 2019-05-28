//
// Created by alex on 5/19/19.
//

#ifndef GUI_STRINGDATA_H
#define GUI_STRINGDATA_H

#include <boost/python.hpp>
#include "TypeData.h"

class StringData: public TypeData
{
public:
    explicit StringData() = default;
    StringData(const std::string& data): m_data{boost::python::object(data)} {}
    StringData(const QString& data): m_data{boost::python::object(data.toUtf8().constData())} {}
    StringData(const Value& data): m_data{data} {}

    StringData(const StringData&) = default;
    StringData(StringData&&) = default;

    QtNodes::NodeDataType type() const override
    {
        return {"string", "string"};
    }

    const std::string& string() const
    {
        return boost::python::extract<std::string>(m_data.m_object);
    }

    QString asText() const
    {
        return QString::fromStdString(boost::python::extract<std::string>(m_data.m_object));
    }

    Value value() override { return m_data; }

private:
    Value m_data;
};


#endif //GUI_STRINGDATA_H
