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
    return QString::fromStdString(m_symbol.prettyName + ':' + m_symbol.module);
}

QString GieNodeDataModel::name() const
{
    return QString::fromStdString(m_symbol.qualifiedName);
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
    return makeTypeData(m_returnType, m_nodeId);
}

void GieNodeDataModel::setInData(std::shared_ptr<QtNodes::NodeData> data, QtNodes::PortIndex port)
{
    if(data == nullptr)
    {
        m_portAssign[port] = PortAssigned::No;
        return;
    }

    auto* typeData = dynamic_cast<TypeData*>(data.get());

    if(typeData->isValueId())
        Q_EMIT edit(typeData->valueId(), port);

    m_portAssign[port] = PortAssigned::Yes;

    if(std::find(m_portAssign.begin(), m_portAssign.end(), PortAssigned::No) == m_portAssign.end())
    {
        modelValidationState = QtNodes::NodeValidationState::Valid;
        modelValidationError = QString();
    }
    else
    {
        modelValidationState = QtNodes::NodeValidationState::Warning;
        modelValidationError = QStringLiteral("Missing or incorrect inputs");
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
