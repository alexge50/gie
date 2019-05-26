//
// Created by alex on 5/26/19.
//

#ifndef GUI_MANAGEDIMAGESOURCEDATAMODEL_H
#define GUI_MANAGEDIMAGESOURCEDATAMODEL_H

#include <nodes/NodeDataModel>
#include "src/gie/types/ImageData.h"

#include "src/Project.h"

class ManagedImageSourceDataModel: public QtNodes::NodeDataModel
{
    Q_OBJECT
public:
    ManagedImageSourceDataModel(const ProjectImage&);
    ~ManagedImageSourceDataModel() override = default;

public:
    QString caption() const override { return m_filename; }
    bool captionVisible() const override { return true; }
    QString name() const override { return QStringLiteral("ManagedImageSource"); }

public:
    QJsonObject save() const override;
    void restore(QJsonObject const &p) override;

public:
    unsigned int nPorts(QtNodes::PortType portType) const override;
    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
    void setInData(std::shared_ptr<QtNodes::NodeData>, int) override { }

    QWidget* embeddedWidget() override { return nullptr; }

private:
    std::shared_ptr<ImageData> m_data;
    QString m_filename;
    QString m_uuid;
};


#endif //GUI_MANAGEDIMAGESOURCEDATAMODEL_H
