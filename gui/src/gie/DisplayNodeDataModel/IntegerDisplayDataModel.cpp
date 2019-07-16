//
// Created by alex on 5/25/19.
//

#include "IntegerDisplayDataModel.h"
#include "src/gie/types/TypeData.h"

IntegerDisplayDataModel::IntegerDisplayDataModel():
        m_label{new QLabel()}
{
    m_label->setMargin(3);
}

QtNodes::NodeDataType IntegerDisplayDataModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    return IntegerTypeData().type();
}

void IntegerDisplayDataModel::displayData(Data data)
{
    long long int number = std::get<long long int>(data);

    m_label->setText(QString::number(number));
}