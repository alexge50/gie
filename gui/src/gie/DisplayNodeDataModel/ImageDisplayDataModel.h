//
// Created by alex on 5/25/19.
//

#ifndef GUI_IMAGEDISPLAYDATAMODEL_H
#define GUI_IMAGEDISPLAYDATAMODEL_H

#include <nodes/NodeDataModel>
#include "src/imageviewer/imageviewer.h"

#include "GieDisplayDataModel.h"

class ImageDisplayDataModel: public GieDisplayDataModel
{
    Q_OBJECT
public:
    ImageDisplayDataModel();
    virtual ~ImageDisplayDataModel() override = default;

public:
    QString caption() const override { return QString("ImageDisplay"); }
    bool captionVisible() const override { return false; }
    QString name() const override { return QString("ImageDisplay"); }

public:
    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    QWidget* embeddedWidget() override { return m_imageViewer; }

public Q_SLOTS:
    virtual void displayData(Data data) override;

private:
    ImageViewer* m_imageViewer;
};


#endif //GUI_IMAGEDISPLAYDATAMODEL_H
