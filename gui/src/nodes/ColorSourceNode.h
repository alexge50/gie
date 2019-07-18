//
// Created by alex on 7/17/19.
//

#ifndef GIE_COLORSOURCENODE_H
#define GIE_COLORSOURCENODE_H

#include "src/nodeeditor/SourceNode.h"
#include "src/nodeeditor/TypeData.h"
#include "src/widgets/colorpicker/colorpicker.h"

class ColorSourceNode: public SourceNode
{
Q_OBJECT
public:
    ColorSourceNode(): m_colorPicker(new ColorPicker())
    {
        m_colorPicker->setMaximumSize(m_colorPicker->sizeHint());

        connect(
                m_colorPicker, &ColorPicker::onColorChanged,
                this, &ColorSourceNode::onColorChanged
        );
    }

    ~ColorSourceNode() override = default;

public:
    QString caption() const override { return QStringLiteral("ColorSource"); }
    bool captionVisible() const override { return false; }
    QString name() const override { return QStringLiteral("ColorSource"); }

public:
    QJsonObject save() const override
    {
        QJsonObject modelJson = NodeDataModel::save();

        modelJson["R"] = m_colorPicker->color().red();
        modelJson["G"] = m_colorPicker->color().green();
        modelJson["B"] = m_colorPicker->color().blue();

        return modelJson;
    }

    void restore(QJsonObject const &p) override
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

            onColorChanged(QColor{
                    r.toInt(),
                    g.toInt(),
                    b.toInt()
            });
        }
    }

public:
    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override
    {
        return ColorTypeData().type();
    }

    std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override
    {
        return std::make_shared<ColorTypeData>();
    }

    QWidget* embeddedWidget() override { return m_colorPicker; }

    Data getData() override
    {
        return Data(Color{
                static_cast<uint8_t>(m_colorPicker->color().red()),
                static_cast<uint8_t>(m_colorPicker->color().green()),
                static_cast<uint8_t>(m_colorPicker->color().blue())
        });
    }

private Q_SLOTS:
    void onColorChanged(const QColor& color)
    {
        Q_EMIT dataChanged(Color{
                static_cast<uint8_t>(color.red()),
                static_cast<uint8_t>(color.green()),
                static_cast<uint8_t>(color.blue())
        });
        ok();
    }

private:
    ColorPicker* m_colorPicker;
};

#endif //GIE_COLORSOURCENODE_H