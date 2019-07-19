//
// Created by alex on 7/17/19.
//

#ifndef GIE_COMPUTENODE_H
#define GIE_COMPUTENODE_H

#include "BaseNode.h"
#include "TypeData.h"

class ComputeNode: public BaseNode
{
public:

    void setInData(std::shared_ptr<QtNodes::NodeData> data, QtNodes::PortIndex port) override
    {
        m_portAssign.resize(nPorts(QtNodes::PortType::In), PortAssigned::No);

        if(data == nullptr)
        {
            m_portAssign[port] = PortAssigned::No;
            return;
        }

        m_portAssign[port] = PortAssigned::Yes;

        if(std::find(m_portAssign.begin(), m_portAssign.end(), PortAssigned::No) == m_portAssign.end())
        {
            ok();
        }
        else
        {
            warning("Missing or incorrect inputs");
        }

        Q_EMIT dataUpdated(0);
    }

public:
    Data getData() override { return {}; }

public Q_SLOTS:
    void displayData(Data data) override {}


protected:
    void invalidateData() final
    {
        Q_EMIT dataInvalidated(0);
    }

    void updateData() final
    {
        Q_EMIT dataUpdated(0);
    }

private:
    enum class PortAssigned: char
    {
        Yes,
        No
    };

    std::vector<PortAssigned> m_portAssign;
};

#endif //GIE_COMPUTENODE_H
