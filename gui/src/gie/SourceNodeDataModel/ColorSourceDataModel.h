//
// Created by alex on 5/24/19.
//

#ifndef GUI_COLORSOURCEDATAMODEL_H
#define GUI_COLORSOURCEDATAMODEL_H

#include <nodes/NodeDataModel>

#include "src/colorpicker/colorpicker.h"
#include "src/gie/types/ColorData.h"

class ColorSourceDataModel: public QtNodes::NodeDataModel
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
    void setInData(std::shared_ptr<QtNodes::NodeData>, int) override { }

    QWidget* embeddedWidget() override { return m_colorPicker; }

Q_SIGNALS:
    void onValueChanged(std::shared_ptr<QtNodes::NodeData>);

private Q_SLOTS:
    void onColorChanged(QColor);

private:
    ColorPicker* m_colorPicker;
    std::shared_ptr<ColorData> m_data;
};


#endif //GUI_COLORSOURCEDATAMODEL_H
