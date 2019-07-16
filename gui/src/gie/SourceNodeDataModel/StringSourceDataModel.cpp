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

    modelJson["string"] = m_lineEdit->text();

    return modelJson;
}


void StringSourceDataModel::restore(QJsonObject const &p)
{
    QJsonValue v = p["string"];

    if (!v.isUndefined())
    {
        QString str = v.toString();
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

    Q_EMIT valueChanged(string.toStdString());
    Q_EMIT dataUpdated(0);
}


QtNodes::NodeDataType StringSourceDataModel::dataType(QtNodes::PortType, QtNodes::PortIndex) const
{
    return StringTypeData().type();
}


std::shared_ptr<QtNodes::NodeData> StringSourceDataModel::outData(QtNodes::PortIndex)
{
    return std::make_shared<StringTypeData>(m_valueId);
}