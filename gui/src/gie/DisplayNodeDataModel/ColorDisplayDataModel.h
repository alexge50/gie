//
// Created by alex on 5/24/19.
//

#ifndef GUI_COLORDISPLAYDATAMODEL_H
#define GUI_COLORDISPLAYDATAMODEL_H

#include <nodes/NodeDataModel>
#include "src/colorsample/colorsample.h"

class ColorDisplayDataModel: public QtNodes::NodeDataModel
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
    unsigned int nPorts(QtNodes::PortType portType) const override;
    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex portIndex) override { return {}; }
    void setInData(std::shared_ptr<QtNodes::NodeData>, QtNodes::PortIndex portIndex) override;

    QWidget* embeddedWidget() override { return m_colorSample; }

    QtNodes::NodeValidationState validationState() const override { return modelValidationState; }
    QString validationMessage() const override { return modelValidationError; }


private:
    QtNodes::NodeValidationState modelValidationState = QtNodes::NodeValidationState::Warning;
    QString modelValidationError = QString("Missing or incorrect inputs");

    ColorSample* m_colorSample;
};


#endif //GUI_COLORDISPLAYDATAMODEL_H
