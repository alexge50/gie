//
// Created by alex on 5/19/19.
//

#include <gie/NodeMetadata.h>
#include <gie/Argument.h>
#include <gie/ScriptGraph/Execute.h>
#undef B0

#include "GieNodeDataModel.h"
#include "TypeData.h"

#include <iostream>

unsigned int GieNodeDataModel::nPorts(QtNodes::PortType portType) const
{
    if(portType == QtNodes::PortType::In)
        return static_cast<unsigned int>(m_metadata.m_arguments.size());
    else return 1;
}

QtNodes::NodeDataType GieNodeDataModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    if(portType == QtNodes::PortType::In)
        return getTypeData(m_metadata.m_arguments[portIndex].m_argumentType);
    return getTypeData(m_metadata.m_returnType);
}

QString GieNodeDataModel::caption() const
{
    return QString::fromStdString(m_metadata.m_functionName);
}

QString GieNodeDataModel::name() const
{
    return QString::fromStdString(m_metadata.m_functionName);
}

bool GieNodeDataModel::portCaptionVisible(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    return true;
}

QString GieNodeDataModel::portCaption(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    if(portType == QtNodes::PortType::In)
        return QString::fromStdString(m_metadata.m_arguments[portIndex].m_argumentName);
    return QString("result");
}

std::shared_ptr<QtNodes::NodeData> GieNodeDataModel::outData(QtNodes::PortIndex port)
{
    auto result = executeNode({{}, m_logic, m_metadata});

    if(result.has_value())
        return std::shared_ptr<QtNodes::NodeData>(extractNodeData(result.value()));
    else return std::shared_ptr<QtNodes::NodeData>();
}

void GieNodeDataModel::setInData(std::shared_ptr<QtNodes::NodeData> data, QtNodes::PortIndex port)
{
    if(!data)
        return;

    m_logic.m_argument[port] = extractGieValue(data);

    if(!std::holds_alternative<NodeId>(m_program.getNode(m_nodeId).m_logic.m_argument[port]))
    {
        auto node = m_program.getNode(m_nodeId);
        node.m_logic.m_argument[port] = m_logic.m_argument[port];
        std::cout << "node edited" << std::endl;
        //m_program.editNode(m_nodeId, node);
    }
}

QtNodes::NodeValidationState GieNodeDataModel::validationState() const
{
    return modelValidationState;
}

QString GieNodeDataModel::validationMessage() const
{
    return modelValidationError;
}
