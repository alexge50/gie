//
// Created by alex on 5/19/19.
//

#ifndef GUI_GIENODEDATAMODEL_H
#define GUI_GIENODEDATAMODEL_H

#include <gie/Node.h>
#include <gie/NodeLogic.h>
#include <gie/Program.h>
#undef B0

#include <nodes/NodeDataModel>
#include <QString>

class GieNodeDataModel: public QtNodes::NodeDataModel
{
    Q_OBJECT

public:
    explicit GieNodeDataModel(Program& program, NodeMetadata metadata):
        m_program{program},
        m_metadata(std::move(metadata)),
        m_logic{std::vector<ArgumentValue>(m_metadata.m_arguments.size(), {NoArgument{}})}
    {
        m_nodeId = program.addNode({{}, m_logic, m_metadata});
    };

    GieNodeDataModel() = delete;

    virtual ~GieNodeDataModel() override
    {
        m_program.removeNode(m_nodeId);
    }

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
    Program& m_program;
    NodeId m_nodeId;
    NodeMetadata m_metadata;
    NodeLogic m_logic;

    QtNodes::NodeValidationState modelValidationState = QtNodes::NodeValidationState::Warning;
    QString modelValidationError = QString("Missing or incorrect inputs");

    friend class Editor;
};


#endif //GUI_GIENODEDATAMODEL_H
