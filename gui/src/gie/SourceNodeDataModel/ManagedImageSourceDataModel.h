//
// Created by alex on 5/26/19.
//

#ifndef GUI_MANAGEDIMAGESOURCEDATAMODEL_H
#define GUI_MANAGEDIMAGESOURCEDATAMODEL_H

#include <nodes/NodeDataModel>
#include "src/gie/types/TypeData.h"
#include "GieSourceDataModel.h"

#include "src/Project.h"

class ManagedImageSourceDataModel: public GieSourceDataModel
{
    Q_OBJECT
public:
    explicit ManagedImageSourceDataModel(const ProjectImage&);
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

    QWidget* embeddedWidget() override { return nullptr; }

    Data getData() override { return {}; }

private:
    QString m_filename;
    QString m_uuid;
    Image m_image;

    friend class Editor;
};


#endif //GUI_MANAGEDIMAGESOURCEDATAMODEL_H
