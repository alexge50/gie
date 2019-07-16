//
// Created by alex on 5/24/19.
//

#ifndef GUI_COLORSOURCEDATAMODEL_H
#define GUI_COLORSOURCEDATAMODEL_H

#include <nodes/NodeDataModel>

#include "src/colorpicker/colorpicker.h"
#include "src/gie/types/TypeData.h"
#include "GieSourceDataModel.h"

class ColorSourceDataModel: public GieSourceDataModel
{
    Q_OBJECT
public:
    ColorSourceDataModel();
    ~ColorSourceDataModel() override = default;

public:
    QString caption() const override { return QStringLiteral("color source"); }
    bool captionVisible() const override { return false; }
    QString name() const override { return QStringLiteral("ColorSource"); }

public:
    QJsonObject save() const override;
    void restore(QJsonObject const &p) override;

public:
    unsigned int nPorts(QtNodes::PortType portType) const override;
    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;

    QWidget* embeddedWidget() override { return m_colorPicker; }

    Data getData() override;

private Q_SLOTS:
    void onColorChanged(QColor);

private:
    ColorPicker* m_colorPicker;
};


#endif //GUI_COLORSOURCEDATAMODEL_H
