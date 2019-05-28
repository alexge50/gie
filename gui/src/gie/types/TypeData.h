//
// Created by alex on 5/19/19.
//

#ifndef GUI_TYPEDATA_H
#define GUI_TYPEDATA_H

#include <gie/Value.h>
#include <nodes/NodeData>

class TypeData: public QtNodes::NodeData
{
public:
    virtual Value value() = 0;
};


#endif //GUI_TYPEDATA_H
