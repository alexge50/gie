//
// Created by alex on 5/19/19.
//

#ifndef GUI_STRINGDATA_H
#define GUI_STRINGDATA_H

#include <nodes/NodeData>

class StringData: public QtNodes::NodeData
{
public:
    explicit StringData() = default;
    StringData(std::string data): m_data{std::move(data)} {}
    StringData(const QString& data): m_data(data.toUtf8().constData()) {}

    StringData(const StringData&) = default;
    StringData(StringData&&) = default;

    QtNodes::NodeDataType type() const override
    {
        return {"string", "string"};
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
    std::string m_data;
};


#endif //GUI_STRINGDATA_H
