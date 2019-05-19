//
// Created by alex on 5/19/19.
//

#ifndef GUI_GIEDATAMODELREGISTRY_H
#define GUI_GIEDATAMODELREGISTRY_H

#include <gie/NodeMetadata.h>
#undef B0

#include <nodes/DataModelRegistry>

class GieDataModelRegistry: public QtNodes::DataModelRegistry
{
public:
    void registerModel(const NodeMetadata&, const QString& category);
};

[[maybe_unused]]
static std::shared_ptr<QtNodes::DataModelRegistry> registerDataModels()
{
    std::shared_ptr<QtNodes::DataModelRegistry> registry(new GieDataModelRegistry, [](auto p){
        delete reinterpret_cast<GieDataModelRegistry*>(p);
    });

    return registry;
}



#endif //GUI_GIEDATAMODELREGISTRY_H
