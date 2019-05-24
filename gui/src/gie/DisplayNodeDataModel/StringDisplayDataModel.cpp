//
// Created by alex on 5/24/19.
//

#include "StringDisplayDataModel.h"
#include "src/gie/types/StringData.h"

StringDisplayDataModel::StringDisplayDataModel():
        m_label{new QLabel()}
{
    m_label->setMargin(3);
}

unsigned int StringDisplayDataModel::nPorts(QtNodes::PortType portType) const
{
    if(portType == QtNodes::PortType::In)
        return 1;
    return 0;
}

QtNodes::NodeDataType StringDisplayDataModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    return StringData().type();
}

void StringDisplayDataModel::setInData(std::shared_ptr<QtNodes::NodeData> input, QtNodes::PortIndex portIndex)
{
    if(auto* data = dynamic_cast<StringData*>(input.get()); data)
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