//
// Created by alex on 5/25/19.
//

#ifndef GUI_PREVIEWIMAGEDISPLAYDATAMODEL_H
#define GUI_PREVIEWIMAGEDISPLAYDATAMODEL_H

#include <nodes/NodeDataModel>
#include "src/imageviewer/imageviewer.h"

#include <QDockWidget>

class PreviewImageDisplayDataModel: public QtNodes::NodeDataModel
{
    Q_OBJECT
public:
    PreviewImageDisplayDataModel();
    virtual ~PreviewImageDisplayDataModel() override = default;

public:
    QString caption() const override { return QString("PreviewImageDisplay"); }
    bool captionVisible() const override { return false; }
    QString name() const override { return QString("PreviewImageDisplay"); }

public:
    unsigned int nPorts(QtNodes::PortType portType) const override;
    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex portIndex) override { return {}; }
    void setInData(std::shared_ptr<QtNodes::NodeData>, QtNodes::PortIndex portIndex) override;

    QWidget* embeddedWidget() override { return nullptr; }

    QtNodes::NodeValidationState validationState() const override { return modelValidationState; }
    QString validationMessage() const override { return modelValidationError; }

    QDockWidget* dockWidget() { return m_dock; }

private:
    QtNodes::NodeValidationState modelValidationState = QtNodes::NodeValidationState::Warning;
    QString modelValidationError = QString("Missing or incorrect inputs");

    QDockWidget* m_dock;
    ImageViewer* m_imageViewer;
};


#endif //GUI_PREVIEWIMAGEDISPLAYDATAMODEL_H
