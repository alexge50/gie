//
// Created by alex on 5/24/19.
//

#ifndef GUI_INTDATA_H
#define GUI_INTDATA_H

#include <nodes/NodeData>

class IntegerData: public QtNodes::NodeData
{
public:
    explicit IntegerData() = default;
    IntegerData(long long int data): m_data{data} {}

    IntegerData(const IntegerData&) = default;
    IntegerData(IntegerData&&) = default;

    QtNodes::NodeDataType type() const override
    {
        return {"integer", "integer"};
    }

    long long int number() const
    {
        return m_data;
    }

    QString asText() const
    {
        return QString::number(m_data);
    }


private:
    long long int m_data;
};

#endif //GUI_INTDATA_H
