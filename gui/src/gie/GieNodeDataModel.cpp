//
// Created by alex on 5/19/19.
//

#include "GieNodeDataModel.h"
#include "TypeData.h"

unsigned int GieNodeDataModel::nPorts(QtNodes::PortType portType) const
{
    if(portType == QtNodes::PortType::In)
        return static_cast<unsigned int>(m_metadata.m_arguments.size());
    else return 1;
}

QtNodes::NodeDataType GieNodeDataModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    if(portType == QtNodes::PortType::In)
        return getTypeData(m_metadata.m_arguments[portIndex]);
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

std::shared_ptr<QtNodes::NodeData> GieNodeDataModel::outData(QtNodes::PortIndex port)
{
    return std::shared_ptr<QtNodes::NodeData>();
}

void GieNodeDataModel::setInData(std::shared_ptr<QtNodes::NodeData> data, QtNodes::PortIndex port)
{

}

QtNodes::NodeValidationState GieNodeDataModel::validationState() const
{
    return NodeDataModel::validationState();
}

QString GieNodeDataModel::validationMessage() const
{
    return NodeDataModel::validationMessage();
}
