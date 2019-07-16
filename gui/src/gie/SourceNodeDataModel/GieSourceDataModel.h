//
// Created by alex on 7/15/19.
//

#ifndef GIE_GIESOURCEDATAMODEL_H
#define GIE_GIESOURCEDATAMODEL_H

#include <QtCore/QObject>
#include <nodes/NodeDataModel>
#include <src/gie/types/TypeData.h>

class GieSourceDataModel: public QtNodes::NodeDataModel
{
    Q_OBJECT

public:
    void setInData(std::shared_ptr<QtNodes::NodeData>, int) override
    {

    }

    virtual Data getData() = 0;

Q_SIGNALS:
    void valueChanged(Data);

protected:
    QUuid m_valueId;
    friend class Editor;
};

#endif //GIE_GIESOURCEDATAMODEL_H
