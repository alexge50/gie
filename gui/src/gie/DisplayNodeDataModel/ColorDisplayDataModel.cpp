//
// Created by alex on 5/24/19.
//

#include "ColorDisplayDataModel.h"
#include "src/gie/types/TypeData.h"

ColorDisplayDataModel::ColorDisplayDataModel():
        m_colorSample{new ColorSample()}
{
    m_colorSample->setMaximumSize(70, 50);
}

QtNodes::NodeDataType ColorDisplayDataModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    return ColorTypeData().type();
}

void ColorDisplayDataModel::displayData(Data data)
{
    Color color = std::get<Color>(data);

    m_colorSample->setColor(QColor(
               color.r,
               color.g,
               color.b)
       );
}
