//
// Created by alex on 5/19/19.
//

#ifndef GUI_GIENODEDATAMODEL_H
#define GUI_GIENODEDATAMODEL_H

#include <gie/Node.h>
#undef B0

#include <nodes/NodeDataModel>
#include <QString>

class GieNodeDataModel: public QtNodes::NodeDataModel
{
    Q_OBJECT

public:
    explicit GieNodeDataModel(const NodeMetadata& metadata): m_metadata(metadata) {}
    GieNodeDataModel() = delete;

    virtual ~GieNodeDataModel() override = default;

    unsigned int nPorts(QtNodes::PortType portType) const override;
    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    QString caption() const override;
    QString name() const override;

    std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
    void setInData(std::shared_ptr<QtNodes::NodeData> data, QtNodes::PortIndex port) override;

    QWidget* embeddedWidget() override { return nullptr; }

    QtNodes::NodeValidationState validationState() const override;

    QString validationMessage() const override;

private:
    NodeId m_nodeId;
    NodeMetadata m_metadata;

    QtNodes::NodeValidationState modelValidationState = QtNodes::NodeValidationState::Warning;
    QString modelValidationError = QString("Missing or incorrect inputs");
};


#endif //GUI_GIENODEDATAMODEL_H
