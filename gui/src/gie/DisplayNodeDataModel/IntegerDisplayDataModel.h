//
// Created by alex on 5/25/19.
//

#ifndef GUI_INTEGERDISPLAYDATAMODEL_H
#define GUI_INTEGERDISPLAYDATAMODEL_H

#include <nodes/NodeDataModel>
#include <QLabel>

class IntegerDisplayDataModel: public QtNodes::NodeDataModel
{
Q_OBJECT
public:
    IntegerDisplayDataModel();
    virtual ~IntegerDisplayDataModel() override = default;

public:
    QString caption() const override { return QString("IntegerDisplay"); }
    bool captionVisible() const override { return false; }
    QString name() const override { return QString("IntegerDisplay"); }

public:
    unsigned int nPorts(QtNodes::PortType portType) const override;
    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex portIndex) override { return {}; }
    void setInData(std::shared_ptr<QtNodes::NodeData>, QtNodes::PortIndex portIndex) override;

    QWidget* embeddedWidget() override { return m_label; }

    QtNodes::NodeValidationState validationState() const override { return modelValidationState; }
    QString validationMessage() const override { return modelValidationError; }


private:
    QtNodes::NodeValidationState modelValidationState = QtNodes::NodeValidationState::Warning;
    QString modelValidationError = QString("Missing or incorrect inputs");

    QLabel* m_label;
};

#endif //GUI_INTEGERDISPLAYDATAMODEL_H
