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

    if (m_data)
    {
        modelJson["R"] = m_data->color().r;
        modelJson["G"] = m_data->color().g;
        modelJson["B"] = m_data->color().b;
    }

    return modelJson;
}

void ColorSourceDataModel::restore(QJsonObject const &p)
{
    QJsonValue r = p["R"];
    QJsonValue g = p["G"];
    QJsonValue b = p["B"];

    if (!r.isUndefined() && !g.isUndefined() && !b.isUndefined())
    {
        Color color{static_cast<uint8_t>(r.toInt()), static_cast<uint8_t>(g.toInt()), static_cast<uint8_t>(b.toInt())};
        m_data = std::make_shared<ColorData>(color);
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
    m_data = std::make_shared<ColorData>(Color{
        static_cast<uint8_t>(color.red()),
        static_cast<uint8_t>(color.green()),
        static_cast<uint8_t>(color.blue())
    });
    Q_EMIT dataUpdated(0);
    Q_EMIT onValueChanged(m_data);
}

QtNodes::NodeDataType ColorSourceDataModel::dataType(QtNodes::PortType, QtNodes::PortIndex) const
{
    return ColorData().type();
}


std::shared_ptr<QtNodes::NodeData> ColorSourceDataModel::outData(QtNodes::PortIndex)
{
    return m_data;
}