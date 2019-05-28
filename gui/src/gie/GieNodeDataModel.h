//
// Created by alex on 5/19/19.
//

#ifndef GUI_GIENODEDATAMODEL_H
#define GUI_GIENODEDATAMODEL_H

#include <gie/Node.h>
#include <gie/NodeLogic.h>
#include <gie/Program.h>

#include <nodes/NodeDataModel>
#include <QString>
#include <QThread>

class GieNodeDataModel: public QtNodes::NodeDataModel
{
    Q_OBJECT

public:
    explicit GieNodeDataModel(Program& program, NodeMetadata metadata);
    GieNodeDataModel() = delete;

    ~GieNodeDataModel() override;

public:
    unsigned int nPorts(QtNodes::PortType portType) const override;
    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    QString caption() const override;
    QString name() const override;

    bool portCaptionVisible(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
    QString portCaption(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
    void setInData(std::shared_ptr<QtNodes::NodeData> data, QtNodes::PortIndex port) override;

    QWidget* embeddedWidget() override { return nullptr; }

    QtNodes::NodeValidationState validationState() const override;

    QString validationMessage() const override;

    auto nodeId() const { return m_nodeId; }

Q_SIGNALS:
    void startWork(const Node& node);

private Q_SLOTS:
    void onWorkFinished(std::shared_ptr<QtNodes::NodeData>);

private:
    QThread m_workerThread;
    Program& m_program;
    NodeId m_nodeId;
    NodeMetadata m_metadata;
    NodeLogic m_logic;

    std::shared_ptr<QtNodes::NodeData> m_result;

    QtNodes::NodeValidationState modelValidationState = QtNodes::NodeValidationState::Warning;
    QString modelValidationError = QString("Missing or incorrect inputs");

    friend class Editor;
};


#endif //GUI_GIENODEDATAMODEL_H
