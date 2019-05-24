//
// Created by alex on 5/25/19.
//

#include "IntegerSourceDataModel.h"

#include <QtCore/QJsonValue>
#include <QtGui/QIntValidator>

IntegerSourceDataModel::IntegerSourceDataModel(): m_lineEdit(new QLineEdit())
{
    m_lineEdit->setValidator(new QIntValidator());
    m_lineEdit->setMaximumSize(m_lineEdit->sizeHint());

    connect(
            m_lineEdit, &QLineEdit::textChanged,
            this, &IntegerSourceDataModel::onTextEdited
    );

    m_lineEdit->setText("0");
}


QJsonObject IntegerSourceDataModel::save() const
{
    QJsonObject modelJson = NodeDataModel::save();

    if (m_data)
        modelJson["number"] = QString::number(m_data->number());

    return modelJson;
}


void IntegerSourceDataModel::restore(QJsonObject const &p)
{
    QJsonValue v = p["number"];

    if (!v.isUndefined())
    {
        QString str = v.toString();

        bool ok;
        long long number = str.toLongLong(&ok);

        if(ok)
        {
            m_data = std::make_shared<IntegerData>(number);
            m_lineEdit->setText(str);
        }
    }
}


unsigned int IntegerSourceDataModel::nPorts(QtNodes::PortType portType) const
{
    if(portType == QtNodes::PortType::In)
        return 0;
    else return 1;
}


void IntegerSourceDataModel::onTextEdited(QString const &string)
{
    Q_UNUSED(string);

    bool ok;
    long long number = string.toLongLong(&ok);

    if(ok)
    {
        m_data = std::make_shared<IntegerData>(number);
        Q_EMIT dataUpdated(0);
        Q_EMIT onValueChanged(m_data);
    }
    else Q_EMIT dataInvalidated(0);
}


QtNodes::NodeDataType IntegerSourceDataModel::dataType(QtNodes::PortType, QtNodes::PortIndex) const
{
    return IntegerData().type();
}


std::shared_ptr<QtNodes::NodeData> IntegerSourceDataModel::outData(QtNodes::PortIndex)
{
    return m_data;
}