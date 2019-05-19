//
// Created by alex on 5/19/19.
//

#ifndef GUI_NUMBERDATA_H
#define GUI_NUMBERDATA_H

#include <nodes/NodeData>

class NumberData: public QtNodes::NodeData
{
public:
    explicit NumberData(QString name): m_name{std::move(name)} {}
    NumberData(QString name, double data): m_name{std::move(name)}, m_data{data} {}

    NumberData(const NumberData&) = default;
    NumberData(NumberData&&) = default;

    QtNodes::NodeDataType type() const override
    {
        return {"double", m_name};
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
    QString m_name;
    double m_data;
};

#endif //GUI_NUMBERDATA_H
