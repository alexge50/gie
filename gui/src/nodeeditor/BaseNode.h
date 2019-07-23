//
// Created by alex on 7/17/19.
//

#ifndef GIE_BASENODE_H
#define GIE_BASENODE_H

#include <nodes/NodeDataModel>
#include <nodes/Node>

#include "TypeData.h"

class BaseNode: public QtNodes::NodeDataModel
{
    Q_OBJECT

public:
    QUuid id() const { return m_id; }

    virtual QString caption() const override = 0;
    virtual QString name() const override = 0;

    virtual unsigned int nPorts(QtNodes::PortType portType) const override = 0;
    virtual QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override = 0;

    virtual QWidget* embeddedWidget() override  = 0;

    virtual void setInData(std::shared_ptr<QtNodes::NodeData>, int) override = 0;
    virtual std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex portIndex) override = 0;

    QtNodes::NodeValidationState validationState() const override { return m_modelValidationState; }
    QString validationMessage() const override { return m_modelValidationError; }

    void error(const QString& message)
    {
        m_modelValidationState = QtNodes::NodeValidationState::Error;
        m_modelValidationError = message;
        invalidateData();
    }

    void warning(const QString& message)
    {
        m_modelValidationState = QtNodes::NodeValidationState::Warning;
        m_modelValidationError = message;
        invalidateData();
    }

    void ok()
    {
        m_modelValidationState = QtNodes::NodeValidationState::Valid;
        m_modelValidationError = "";
        updateData();
    }

Q_SIGNALS:
    void dataChanged(Data);

public:
    virtual Data getData() = 0;

public Q_SLOTS:
    virtual void displayData(Data data) = 0;

protected:
    virtual void invalidateData() = 0;
    virtual void updateData() = 0;

private:
    QtNodes::NodeValidationState m_modelValidationState = QtNodes::NodeValidationState::Warning;
    QString m_modelValidationError = QString("Missing or incorrect inputs");

    QUuid m_id;
    friend class NodeEditor;
};

#endif //GIE_BASENODE_H
