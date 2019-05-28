//
// Created by alex on 5/28/19.
//

#include "NodeProcessingWorker.h"

#include "src/gie/types/ExtractTypes.h"
#include <gie/ScriptGraph/Execute.h>

void NodeProcessingWorker::startNodeProcessing(const Node& node)
{
    auto releaseGIL = ScopedGILRelease();

    auto result = executeNode(node);

    auto resultPtr = [](const auto& result)
    {
        if(result.has_value())
            return extractNodeData(result.value());
        else return std::shared_ptr<QtNodes::NodeData>{};
    }(result);

    Q_EMIT resultReady(std::move(resultPtr));
}