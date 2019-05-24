//
// Created by alex on 5/24/19.
//

#include "ColorDisplayDataModel.h"
#include "src/gie/types/ColorData.h"

ColorDisplayDataModel::ColorDisplayDataModel():
        m_colorSample{new ColorSample()}
{
    m_colorSample->setMaximumSize(70, 50);
}

unsigned int ColorDisplayDataModel::nPorts(QtNodes::PortType portType) const
{
    if(portType == QtNodes::PortType::In)
        return 1;
    return 0;
}

QtNodes::NodeDataType ColorDisplayDataModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    return ColorData().type();
}

void ColorDisplayDataModel::setInData(std::shared_ptr<QtNodes::NodeData> input, QtNodes::PortIndex portIndex)
{
    if(auto* data = dynamic_cast<ColorData*>(input.get()); data)
    {
        modelValidationState = QtNodes::NodeValidationState::Valid;
        modelValidationError = QString();

        auto color = data->color();
        m_colorSample->setColor(QColor(
                color.r,
                color.g,
                color.b)
        );
    }
    else
    {
        modelValidationState = QtNodes::NodeValidationState::Valid;
        modelValidationError = QStringLiteral("Missing or incorrect inputs");

        m_colorSample->setColor(QColor(0, 0, 0));
    }
}
