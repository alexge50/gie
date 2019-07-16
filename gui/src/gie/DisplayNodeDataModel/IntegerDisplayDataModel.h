//
// Created by alex on 5/25/19.
//

#ifndef GUI_INTEGERDISPLAYDATAMODEL_H
#define GUI_INTEGERDISPLAYDATAMODEL_H

#include <nodes/NodeDataModel>
#include <QLabel>

#include "GieDisplayDataModel.h"

class IntegerDisplayDataModel: public GieDisplayDataModel
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
    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
    QWidget* embeddedWidget() override { return m_label; }

public Q_SLOTS:
    void displayData(Data data) override;

private:
    QLabel* m_label;
};

#endif //GUI_INTEGERDISPLAYDATAMODEL_H
