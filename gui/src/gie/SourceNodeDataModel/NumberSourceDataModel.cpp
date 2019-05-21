//
// Created by alex on 5/20/19.
//

#include "NumberSourceDataModel.h"

#include <QtCore/QJsonValue>
#include <QtGui/QDoubleValidator>

NumberSourceDataModel::NumberSourceDataModel(): m_lineEdit(new QLineEdit())
{
    m_lineEdit->setValidator(new QDoubleValidator());
    m_lineEdit->setMaximumSize(m_lineEdit->sizeHint());

    connect(
            m_lineEdit, &QLineEdit::textChanged,
            this, &NumberSourceDataModel::onTextEdited
    );

    m_lineEdit->setText("0.0");
}


QJsonObject NumberSourceDataModel::save() const
{
    QJsonObject modelJson = NodeDataModel::save();

    if (m_data)
        modelJson["number"] = QString::number(m_data->number());

    return modelJson;
}


void NumberSourceDataModel::restore(QJsonObject const &p)
{
    QJsonValue v = p["number"];

    if (!v.isUndefined())
    {
        QString str = v.toString();

        bool ok;
        double number = str.toDouble(&ok);

        if(ok)
        {
            m_data = std::make_shared<NumberData>(number);
            m_lineEdit->setText(str);
        }
    }
}


unsigned int NumberSourceDataModel::nPorts(QtNodes::PortType portType) const
{
    if(portType == QtNodes::PortType::In)
        return 0;
    else return 1;
}


void NumberSourceDataModel::onTextEdited(QString const &string)
{
    Q_UNUSED(string);

    bool ok;
    double number = string.toDouble(&ok);

    if(ok)
    {
        m_data = std::make_shared<NumberData>(number);
        Q_EMIT dataUpdated(0);
        Q_EMIT onValueChanged(m_data);
    }
    else Q_EMIT dataInvalidated(0);
}


QtNodes::NodeDataType NumberSourceDataModel::dataType(QtNodes::PortType, QtNodes::PortIndex) const
{
    return NumberData().type();
}


std::shared_ptr<QtNodes::NodeData> NumberSourceDataModel::outData(QtNodes::PortIndex)
{
    return m_data;
}