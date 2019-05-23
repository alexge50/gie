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

#include "SourceNodeDataModel/NumberSourceDataModel.h"
#include "SourceNodeDataModel/StringSourceDataModel.h"

class GieDataModelRegistry: public QtNodes::DataModelRegistry
{
public:
    GieDataModelRegistry(Program& program): m_program{program} {}

    void registerModel(const NodeMetadata&, const QString& category);

private:
    Program& m_program;
};

[[maybe_unused]]
static std::shared_ptr<QtNodes::DataModelRegistry> registerDataModels(Program& program)
{
    std::shared_ptr<GieDataModelRegistry> registry(new GieDataModelRegistry(program), [](auto p){
        delete reinterpret_cast<GieDataModelRegistry*>(p);
    });

    for(const auto& symbol: program.context().importedSymbols())
        registry->registerModel(fetchMetadata(program.context(), symbol.qualifiedName), "operators");

    static_cast<QtNodes::DataModelRegistry*>(registry.get())->registerModel<StringSourceDataModel>("source");
    static_cast<QtNodes::DataModelRegistry*>(registry.get())->registerModel<NumberSourceDataModel>("source");

    return registry;
}


#endif //GUI_GIEDATAMODELREGISTRY_H
