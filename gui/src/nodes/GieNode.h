//
// Created by alex on 7/17/19.
//

#ifndef GIE_GIENODE_H
#define GIE_GIENODE_H

#include <src/nodeeditor/ComputeNode.h>
#include <src/nodeeditor/ExtractTypes.h>
#include <src/Gie.h>

class GieNode: public ComputeNode
{
Q_OBJECT

public:

    explicit GieNode(GieSymbol symbol):
            m_arguments{std::move(symbol.arguments)},
            m_returnType{std::move(symbol.returnType)},
            m_symbol{std::move(symbol.symbol)}
    {
    }

    GieNode() = delete;

    ~GieNode() override
    {
    }

    unsigned int nPorts(QtNodes::PortType portType) const override
    {
        if(portType == QtNodes::PortType::In)
            return static_cast<unsigned int>(m_arguments.size());
        else return 1;
    }

    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override
    {
        if(portType == QtNodes::PortType::In)
            return getTypeData(m_arguments[portIndex].m_argumentType);
        return getTypeData(m_returnType);
    }

    QString caption() const override
    {
        return QString::fromStdString(m_symbol.prettyName + ':' + m_symbol.module);
    }

    QString name() const override
    {
        return QString::fromStdString(m_symbol.qualifiedName);
    }

    bool portCaptionVisible(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override
    {
        return true;
    }

    QString portCaption(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override
    {
        if(portType == QtNodes::PortType::In)
            return QString::fromStdString(m_arguments[portIndex].m_argumentName);
        return QString("result");
    }

    std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override
    {
        return makeTypeData(m_returnType);
    }

    QWidget* embeddedWidget() override { return nullptr; }

    const auto& symbol() const { return m_symbol; }

private:
    GieNodeId m_nodeId;
    std::vector<ArgumentMetadata> m_arguments;
    Type m_returnType;
    SymbolName m_symbol;

    friend class Editor;
};


#endif //GIE_GIENODE_H
