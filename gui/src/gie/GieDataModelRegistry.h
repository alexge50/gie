//
// Created by alex on 5/19/19.
//

#ifndef GUI_GIEDATAMODELREGISTRY_H
#define GUI_GIEDATAMODELREGISTRY_H

#include <gie/NodeUtil.h>
#include <gie/NodeMetadata.h>
#include <gie/Program.h>
#undef B0

#include <nodes/DataModelRegistry>


class GieDataModelRegistry: public QtNodes::DataModelRegistry
{
public:
    void registerModel(const NodeMetadata&, const QString& category);
};

[[maybe_unused]]
static std::shared_ptr<QtNodes::DataModelRegistry> registerDataModels(Program& program)
{
    std::shared_ptr<GieDataModelRegistry> registry(new GieDataModelRegistry, [](auto p){
        delete reinterpret_cast<GieDataModelRegistry*>(p);
    });

    for(const auto& name: program.context().importedSymbols())
        registry->registerModel(fetchMetadata(program.context(), name), "operators");

    return registry;
}


#endif //GUI_GIEDATAMODELREGISTRY_H
