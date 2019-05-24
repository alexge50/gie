//
// Created by alex on 5/19/19.
//

#include <gie/NodeMetadata.h>
#include <gie/Argument.h>
#include <gie/ScriptGraph/Execute.h>
#undef B0

#include "GieNodeDataModel.h"
#include "src/gie/types/TypeData.h"

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
    return QString::fromStdString(m_metadata.m_symbol.prettyName + ":" + m_metadata.m_symbol.module);
}

QString GieNodeDataModel::name() const
{
    return QString::fromStdString(m_metadata.m_symbol.qualifiedName);
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
        node.m_logic.m_argument[port] =  m_logic.m_argument[port];
        m_program.editNode(m_nodeId, node);
    }

    auto allPortsAssigned = std::find_if(
            m_logic.m_argument.begin(),
            m_logic.m_argument.end(),
            [](const auto& x)
            {
                return std::holds_alternative<NoArgument>(x);
            }) == m_logic.m_argument.end();

    if(allPortsAssigned)
    {
        modelValidationState = QtNodes::NodeValidationState::Valid;
        modelValidationError = QString();
    }
    else
    {
        modelValidationState = QtNodes::NodeValidationState::Warning;
        modelValidationError = QString("Missing or incorrect inputs");
    }

    Q_EMIT dataUpdated(0);

}

QtNodes::NodeValidationState GieNodeDataModel::validationState() const
{
    return modelValidationState;
}

QString GieNodeDataModel::validationMessage() const
{
    return modelValidationError;
}
