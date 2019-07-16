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

    modelJson["number"] = m_lineEdit->text();

    return modelJson;
}


void IntegerSourceDataModel::restore(QJsonObject const &p)
{
    QJsonValue v = p["number"];

    if (!v.isUndefined())
    {
        QString str = v.toString();

        bool ok{};
        str.toLongLong(&ok);

        if(ok)
        {
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
    [[maybe_unused]]long long number = string.toLongLong(&ok);

    if(ok)
    {
        Q_EMIT valueChanged(number);
        Q_EMIT dataUpdated(0);
    }
    else Q_EMIT dataInvalidated(0);
}


QtNodes::NodeDataType IntegerSourceDataModel::dataType(QtNodes::PortType, QtNodes::PortIndex) const
{
    return IntegerTypeData().type();
}


std::shared_ptr<QtNodes::NodeData> IntegerSourceDataModel::outData(QtNodes::PortIndex)
{
    return std::make_shared<IntegerTypeData>(m_valueId);
}

Data IntegerSourceDataModel::getData()
{
    return m_lineEdit->text().toLongLong();
}
