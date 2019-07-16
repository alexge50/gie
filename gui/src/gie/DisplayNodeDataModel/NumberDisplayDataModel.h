//
// Created by alex on 5/24/19.
//

#ifndef GUI_NUMBERDISPLAYDATAMODEL_H
#define GUI_NUMBERDISPLAYDATAMODEL_H

#include <nodes/NodeDataModel>
#include <QLabel>
#include "GieDisplayDataModel.h"

class NumberDisplayDataModel: public GieDisplayDataModel
{
    Q_OBJECT
public:
    NumberDisplayDataModel();
    virtual ~NumberDisplayDataModel() override = default;

public:
    QString caption() const override { return QString("NumberDisplay"); }
    bool captionVisible() const override { return false; }
    QString name() const override { return QString("NumberDisplay"); }

public:
    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
    QWidget* embeddedWidget() override { return m_label; }

public Q_SLOTS:
    void displayData(Data data) override;

private:
    QLabel* m_label;
};


#endif //GUI_NUMBERDISPLAYDATAMODEL_H
