//
// Created by alex on 5/19/19.
//

#ifndef GUI_NUMBERDATA_H
#define GUI_NUMBERDATA_H

#include <nodes/NodeData>

class NumberData: public QtNodes::NodeData
{
public:
    explicit NumberData() = default;
    NumberData(double data): m_data{data} {}

    NumberData(const NumberData&) = default;
    NumberData(NumberData&&) = default;

    QtNodes::NodeDataType type() const override
    {
        return {"double", "double"};
    }

    double number() const
    {
        return m_data;
    }

    QString asText() const
    {
        return QString::number(m_data);
    }


private:
    double m_data;
};

#endif //GUI_NUMBERDATA_H
