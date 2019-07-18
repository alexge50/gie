//
// Created by alex on 7/17/19.
//

#ifndef GIE_COLORDISPLAYNODE_H
#define GIE_COLORDISPLAYNODE_H

#include "src/nodeeditor/DisplayNode.h"
#include "src/widgets/colorsample/colorsample.h"

class ColorDisplayNode: public DisplayNode
{
Q_OBJECT
public:
    ColorDisplayNode():
            m_colorSample{new ColorSample()}
    {
        m_colorSample->setMaximumSize(70, 50);
    }

    virtual ~ColorDisplayNode() override = default;

public:
    QString caption() const override { return QString("ColorDisplay"); }
    bool captionVisible() const override { return false; }
    QString name() const override { return QString("ColorDisplay"); }

public:
    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override
    {
        return ColorTypeData().type();
    }

    QWidget* embeddedWidget() override { return m_colorSample; }

public Q_SLOTS:
    void displayData(Data data) override
    {
        Color color = std::get<Color>(data);

        m_colorSample->setColor(QColor(
                color.r,
                color.g,
                color.b)
        );
    };

private:
    ColorSample* m_colorSample;
};

#endif //GIE_COLORDISPLAYNODE_H
