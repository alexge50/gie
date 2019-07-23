//
// Created by alex on 7/17/19.
//

#ifndef GIE_DISPLAYNODE_H
#define GIE_DISPLAYNODE_H

#include "BaseNode.h"

class DisplayNode: public BaseNode
{
    Q_OBJECT

public:
    std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex portIndex) override { return {}; }

    unsigned int nPorts(QtNodes::PortType portType) const override
    {
        if(portType == QtNodes::PortType::In)
            return 1;
        return 0;
    }

    void setInData(std::shared_ptr<QtNodes::NodeData> data, QtNodes::PortIndex portIndex) override
    {
        if(data)
            ok();
        else
            warning("Missing inputs");
    }

public:
    Data getData() override { return {}; }

protected:
    void invalidateData() final {}

    void updateData() final {}
};

#endif //GIE_DISPLAYNODE_H
