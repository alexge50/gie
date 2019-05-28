//
// Created by alex on 5/28/19.
//

#ifndef GUI_NODEPROCESSINGWORKER_H
#define GUI_NODEPROCESSINGWORKER_H

#include <memory>

#include <QObject>
#include <nodes/NodeData>

#include <gie/Node.h>

#include "src/ScopedGILRelease.h"

class NodeProcessingWorker: public QObject
{
    Q_OBJECT
public Q_SLOTS:
    void startNodeProcessing(const Node& node);

Q_SIGNALS:
    void resultReady(std::shared_ptr<QtNodes::NodeData>);

private:
    ScopedGILRelease release;
};


#endif //GUI_NODEPROCESSINGWORKER_H
