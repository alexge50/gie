//
// Created by alex on 5/19/19.
//

#ifndef GUI_STRINGDATA_H
#define GUI_STRINGDATA_H

#include <nodes/NodeData>

class StringData: public QtNodes::NodeData
{
public:
    explicit StringData(QString name): m_name{std::move(name)} {}
    StringData(QString name, std::string data): m_name{std::move(name)}, m_data{std::move(data)} {}

    StringData(const StringData&) = default;
    StringData(StringData&&) = default;

    QtNodes::NodeDataType type() const override
    {
        return {"string", m_name};
    }

    const std::string& string() const
    {
        return m_data;
    }

    QString asText() const
    {
        return QString::fromStdString(m_data);
    }


private:
    QString m_name;
    std::string m_data;
};


#endif //GUI_STRINGDATA_H
