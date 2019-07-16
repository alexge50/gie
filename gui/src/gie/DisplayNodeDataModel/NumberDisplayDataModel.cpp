//
// Created by alex on 5/24/19.
//

#include "NumberDisplayDataModel.h"
#include "src/gie/types/TypeData.h"

NumberDisplayDataModel::NumberDisplayDataModel():
    m_label{new QLabel()}
{
    m_label->setMargin(3);
}


QtNodes::NodeDataType NumberDisplayDataModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    return NumberTypeData().type();
}

void NumberDisplayDataModel::displayData(Data data)
{
    double number = std::get<double>(data);

    m_label->setText(QString::number(number));
}