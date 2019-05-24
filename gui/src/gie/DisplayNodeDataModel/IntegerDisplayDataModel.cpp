//
// Created by alex on 5/25/19.
//

#include "IntegerDisplayDataModel.h"
#include "src/gie/types/IntegerData.h"

IntegerDisplayDataModel::IntegerDisplayDataModel():
        m_label{new QLabel()}
{
    m_label->setMargin(3);
}

unsigned int IntegerDisplayDataModel::nPorts(QtNodes::PortType portType) const
{
    if(portType == QtNodes::PortType::In)
        return 1;
    return 0;
}

QtNodes::NodeDataType IntegerDisplayDataModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    return IntegerData().type();
}

void IntegerDisplayDataModel::setInData(std::shared_ptr<QtNodes::NodeData> input, QtNodes::PortIndex portIndex)
{
    if(auto* data = dynamic_cast<IntegerData*>(input.get()); data)
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
