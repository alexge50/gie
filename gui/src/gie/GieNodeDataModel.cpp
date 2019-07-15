//
// Created by alex on 5/19/19.
//

#include <gie/Node.h>
#include <gie/Argument.h>
#include <gie/ScriptGraph/Execute.h>

#include "GieNodeDataModel.h"
#include "src/gie/types/ExtractTypes.h"

unsigned int GieNodeDataModel::nPorts(QtNodes::PortType portType) const
{
    if(portType == QtNodes::PortType::In)
        return static_cast<unsigned int>(m_arguments.size());
    else return 1;
}

QtNodes::NodeDataType GieNodeDataModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    if(portType == QtNodes::PortType::In)
        return getTypeData(m_arguments[portIndex].m_argumentType);
    return getTypeData(m_returnType);
}

QString GieNodeDataModel::caption() const
{
    return QString::fromStdString(m_symbol.qualifiedName);
}

QString GieNodeDataModel::name() const
{
    return QString::fromStdString(m_symbol.prettyName + ':' + m_symbol.module);
}

bool GieNodeDataModel::portCaptionVisible(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    return true;
}

QString GieNodeDataModel::portCaption(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    if(portType == QtNodes::PortType::In)
        return QString::fromStdString(m_arguments[portIndex].m_argumentName);
    return QString("result");
}

std::shared_ptr<QtNodes::NodeData> GieNodeDataModel::outData(QtNodes::PortIndex port)
{
    return m_result;
}

void GieNodeDataModel::setInData(std::shared_ptr<QtNodes::NodeData> data, QtNodes::PortIndex port)
{
    if(!data)
        return;

    /*if(!std::holds_alternative<NodeId>(m_program.getNode(m_nodeId).m_logic.m_argument[port]))
    {
        //... emit value change
    }

    // all ports assigned

    //only validation, data will be given directly to the viewer

    if(allPortsAssigned)
    {
        modelValidationState = QtNodes::NodeValidationState::Valid;
        modelValidationError = QString();

        auto result = executeNode({{}, m_logic, m_metadata});

        if(result.has_value())
            m_result = extractNodeData(m_metadata.m_returnType, result.value());
        else
        {
            modelValidationState = QtNodes::NodeValidationState::Warning;
            modelValidationError = QString("Missing or incorrect inputs");
            m_result = std::shared_ptr<QtNodes::NodeData>();
        }
    }
    else
    {
        modelValidationState = QtNodes::NodeValidationState::Warning;
        modelValidationError = QString("Missing or incorrect inputs");
    }

    Q_EMIT dataUpdated(0);*/

}

QtNodes::NodeValidationState GieNodeDataModel::validationState() const
{
    return modelValidationState;
}

QString GieNodeDataModel::validationMessage() const
{
    return modelValidationError;
}
