//
// Created by alex on 5/19/19.
//

#ifndef GUI_GIENODEDATAMODEL_H
#define GUI_GIENODEDATAMODEL_H

#include <gie/Node.h>
#include <gie/Program.h>

#include <nodes/NodeDataModel>
#include <QString>

class GieNodeDataModel: public QtNodes::NodeDataModel
{
    Q_OBJECT

public:

    explicit GieNodeDataModel(Symbol symbol, const NodeType* type):
        m_arguments{type->m_arguments},
        m_returnType{type->m_returnType},
        m_symbol{std::move(symbol)}
    {}

    GieNodeDataModel() = delete;

    ~GieNodeDataModel() override
    {
    }

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

private:
    NodeId m_nodeId;
    std::vector<ArgumentMetadata> m_arguments;
    Type m_returnType;
    Symbol m_symbol;

    std::shared_ptr<QtNodes::NodeData> m_result;

    QtNodes::NodeValidationState modelValidationState = QtNodes::NodeValidationState::Warning;
    QString modelValidationError = QString("Missing or incorrect inputs");

    friend class Editor;
};


#endif //GUI_GIENODEDATAMODEL_H
