//
// Created by alex on 5/24/19.
//

#include "StringDisplayDataModel.h"
#include "src/gie/types/TypeData.h"

StringDisplayDataModel::StringDisplayDataModel():
        m_label{new QLabel()}
{
    m_label->setMargin(3);
}

QtNodes::NodeDataType StringDisplayDataModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    return StringTypeData().type();
}

void StringDisplayDataModel::displayData(Data data)
{
    const std::string& text = std::get<std::string>(data);
    m_label->setText(QString::fromStdString(text));
}
