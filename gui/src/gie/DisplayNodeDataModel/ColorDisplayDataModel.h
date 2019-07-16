//
// Created by alex on 5/24/19.
//

#ifndef GUI_COLORDISPLAYDATAMODEL_H
#define GUI_COLORDISPLAYDATAMODEL_H

#include <nodes/NodeDataModel>
#include "src/colorsample/colorsample.h"
#include "GieDisplayDataModel.h"

class ColorDisplayDataModel: public GieDisplayDataModel
{
    Q_OBJECT
public:
    ColorDisplayDataModel();
    virtual ~ColorDisplayDataModel() override = default;

public:
    QString caption() const override { return QString("ColorDisplay"); }
    bool captionVisible() const override { return false; }
    QString name() const override { return QString("ColorDisplay"); }

public:
    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
    QWidget* embeddedWidget() override { return m_colorSample; }

public Q_SLOTS:
    void displayData(Data data) override;

private:
    ColorSample* m_colorSample;
};


#endif //GUI_COLORDISPLAYDATAMODEL_H
