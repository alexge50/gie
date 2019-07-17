//
// Created by alex on 5/28/19.
//

#ifndef GUI_TARGETEXPORTIMAGEDATAMODEL_H
#define GUI_TARGETEXPORTIMAGEDATAMODEL_H

#include <nodes/NodeDataModel>
#include <QLineEdit>
#include "src/imageviewer/imageviewer.h"

#include <QDockWidget>
#include "GieDisplayDataModel.h"

class TargetExportImageDataModel: public GieDisplayDataModel
{
    Q_OBJECT
public:
    TargetExportImageDataModel();
    virtual ~TargetExportImageDataModel() override = default;

public:
    QString caption() const override { return QString("export target"); }
    bool captionVisible() const override { return true; }
    QString name() const override { return QString("TargetExportImage"); }

public:
    QJsonObject save() const override;
    void restore(const QJsonObject& p) override;

    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    QWidget* embeddedWidget() override { return m_targetNameEdit; }

    const QString& getTargetName() const { return m_targetName; }
    QString getId() const { return m_id.toString(); };
    QDockWidget* dockWidget() { return m_dock; }

    const QImage& getImage() const { return m_image; }

public Q_SLOTS:
    void displayData(Data data) override;

Q_SIGNALS:
    void targetNameChanged(const QUuid& id, const QString&);

private Q_SLOTS:
    void onTargetNameChanged(const QString&);

private:
    QLineEdit* m_targetNameEdit;
    QString m_targetName;
    QUuid m_id;

    QImage m_image;

    QDockWidget* m_dock;
    ImageViewer* m_imageViewer;
};


#endif //GUI_TARGETEXPORTIMAGEDATAMODEL_H
