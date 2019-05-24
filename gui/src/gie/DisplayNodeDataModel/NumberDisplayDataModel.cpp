//
// Created by alex on 5/24/19.
//

#include "NumberDisplayDataModel.h"
#include "src/gie/types/NumberData.h"

NumberDisplayDataModel::NumberDisplayDataModel():
    m_label{new QLabel()}
{
    m_label->setMargin(3);
}

unsigned int NumberDisplayDataModel::nPorts(QtNodes::PortType portType) const
{
    if(portType == QtNodes::PortType::In)
        return 1;
    return 0;
}

QtNodes::NodeDataType NumberDisplayDataModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    return NumberData().type();
}

void NumberDisplayDataModel::setInData(std::shared_ptr<QtNodes::NodeData> input, QtNodes::PortIndex portIndex)
{
    if(auto* data = dynamic_cast<NumberData*>(input.get()); data)
    {
        modelValidationState = QtNodes::NodeValidationState::Valid;
        modelValidationError = QString();

        m_label->setText(data->asText());
    }
    else
    {
        modelValidationState = QtNodes::NodeValidationState::Valid;
        modelValidationError = QStringLiteral("Missing or incorrect inputs");

        m_label->clear();
    }

    m_label->adjustSize();
}
