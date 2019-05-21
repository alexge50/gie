//
// Created by alex on 5/20/19.
//

#include "StringSourceDataModel.h"

#include <QtCore/QJsonValue>
#include <QtGui/QDoubleValidator>

StringSourceDataModel::StringSourceDataModel(): m_lineEdit(new QLineEdit())
{
    m_lineEdit->setMaximumSize(m_lineEdit->sizeHint());

    connect(
            m_lineEdit, &QLineEdit::textChanged,
            this, &StringSourceDataModel::onTextEdited
    );

    m_lineEdit->setText("string");
}


QJsonObject StringSourceDataModel::save() const
{
    QJsonObject modelJson = NodeDataModel::save();

    if (m_data)
        modelJson["string"] = m_data->asText();

    return modelJson;
}


void StringSourceDataModel::restore(QJsonObject const &p)
{
    QJsonValue v = p["string"];

    if (!v.isUndefined())
    {
        QString str = v.toString();

        m_data = std::make_shared<StringData>(str);
        m_lineEdit->setText(str);
    }
}


unsigned int StringSourceDataModel::nPorts(QtNodes::PortType portType) const
{
    if(portType == QtNodes::PortType::In)
        return 0;
    else return 1;
}


void StringSourceDataModel::onTextEdited(QString const &string)
{
    Q_UNUSED(string);

    m_data = std::make_shared<StringData>(string);
    Q_EMIT dataUpdated(0);
    Q_EMIT onValueChanged(m_data);
}


QtNodes::NodeDataType StringSourceDataModel::dataType(QtNodes::PortType, QtNodes::PortIndex) const
{
    return StringData().type();
}


std::shared_ptr<QtNodes::NodeData> StringSourceDataModel::outData(QtNodes::PortIndex)
{
    return m_data;
}