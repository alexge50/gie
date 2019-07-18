//
// Created by alex on 7/17/19.
//

#ifndef GIE_SOURCENODE_H
#define GIE_SOURCENODE_H

#include "BaseNode.h"

class SourceNode: public BaseNode
{
    Q_OBJECT

public:
    void setInData(std::shared_ptr<QtNodes::NodeData>, int) override
    {

    }

    unsigned int nPorts(QtNodes::PortType portType) const override
    {
        if(portType == QtNodes::PortType::In)
            return 0;
        else return 1;
    }

public Q_SLOTS:
    virtual void displayData(Data) override {};

protected:
    void invalidateData() final
    {
        Q_EMIT dataInvalidated(0);
    }

    void updateData() final
    {
        Q_EMIT dataUpdated(0);
    }
};

#endif //GIE_SOURCENODE_H
