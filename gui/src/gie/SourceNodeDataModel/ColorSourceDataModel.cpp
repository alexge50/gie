//
// Created by alex on 5/24/19.
//

#include "ColorSourceDataModel.h"

ColorSourceDataModel::ColorSourceDataModel(): m_colorPicker(new ColorPicker())
{
    m_colorPicker->setMaximumSize(m_colorPicker->sizeHint());

    connect(
            m_colorPicker, &ColorPicker::onColorChanged,
            this, &ColorSourceDataModel::onColorChanged
    );
}

QJsonObject ColorSourceDataModel::save() const
{
    QJsonObject modelJson = NodeDataModel::save();

    modelJson["R"] = m_colorPicker->color().red();
    modelJson["G"] = m_colorPicker->color().green();
    modelJson["B"] = m_colorPicker->color().blue();

    return modelJson;
}

void ColorSourceDataModel::restore(QJsonObject const &p)
{
    QJsonValue r = p["R"];
    QJsonValue g = p["G"];
    QJsonValue b = p["B"];

    if (!r.isUndefined() && !g.isUndefined() && !b.isUndefined())
    {
        m_colorPicker->setColor(QColor{
            r.toInt(),
            g.toInt(),
            b.toInt()
        });
    }
}

unsigned int ColorSourceDataModel::nPorts(QtNodes::PortType portType) const
{
    if(portType == QtNodes::PortType::In)
        return 0;
    else return 1;
}

void ColorSourceDataModel::onColorChanged(QColor color)
{
    Q_EMIT valueChanged(Color{
                static_cast<uint8_t>(color.red()),
                static_cast<uint8_t>(color.green()),
                static_cast<uint8_t>(color.blue())
            });
    Q_EMIT dataUpdated(0);

}

QtNodes::NodeDataType ColorSourceDataModel::dataType(QtNodes::PortType, QtNodes::PortIndex) const
{
    return ColorTypeData().type();
}


std::shared_ptr<QtNodes::NodeData> ColorSourceDataModel::outData(QtNodes::PortIndex)
{
    return std::make_shared<ColorTypeData>(m_valueId);
}