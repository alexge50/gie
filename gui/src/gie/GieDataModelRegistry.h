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
#include "SourceNodeDataModel/IntegerSourceDataModel.h"
#include "SourceNodeDataModel/StringSourceDataModel.h"
#include "SourceNodeDataModel/ColorSourceDataModel.h"
#include "SourceNodeDataModel/ImageSourceDataModel.h"

#include "DisplayNodeDataModel/NumberDisplayDataModel.h"
#include "DisplayNodeDataModel/IntegerDisplayDataModel.h"
#include "DisplayNodeDataModel/StringDisplayDataModel.h"
#include "DisplayNodeDataModel/ColorDisplayDataModel.h"
#include "DisplayNodeDataModel/ImageDisplayDataModel.h"
#include "DisplayNodeDataModel/PreviewImageDisplayDataModel.h"

class GieDataModelRegistry: public QtNodes::DataModelRegistry
{
public:
    GieDataModelRegistry(Program& program): m_program{program} {}

    void registerModel(const NodeMetadata&, const QString& category);

private:
    Program& m_program;
};

[[maybe_unused]]
static std::shared_ptr<GieDataModelRegistry> registerDataModels(Program& program)
{
    std::shared_ptr<GieDataModelRegistry> registry(new GieDataModelRegistry(program), [](auto p){
        delete reinterpret_cast<GieDataModelRegistry*>(p);
    });

    for(const auto& symbol: program.context().importedSymbols())
        registry->registerModel(fetchMetadata(program.context(), symbol.qualifiedName), QString::fromStdString(symbol.module));

    static_cast<QtNodes::DataModelRegistry*>(registry.get())->registerModel<StringSourceDataModel>("source");
    static_cast<QtNodes::DataModelRegistry*>(registry.get())->registerModel<NumberSourceDataModel>("source");
    static_cast<QtNodes::DataModelRegistry*>(registry.get())->registerModel<IntegerSourceDataModel>("source");
    static_cast<QtNodes::DataModelRegistry*>(registry.get())->registerModel<ColorSourceDataModel>("source");
    static_cast<QtNodes::DataModelRegistry*>(registry.get())->registerModel<ImageSourceDataModel>("source");

    static_cast<QtNodes::DataModelRegistry*>(registry.get())->registerModel<NumberDisplayDataModel>("display");
    static_cast<QtNodes::DataModelRegistry*>(registry.get())->registerModel<IntegerDisplayDataModel>("display");
    static_cast<QtNodes::DataModelRegistry*>(registry.get())->registerModel<StringDisplayDataModel>("display");
    static_cast<QtNodes::DataModelRegistry*>(registry.get())->registerModel<ColorDisplayDataModel>("display");
    static_cast<QtNodes::DataModelRegistry*>(registry.get())->registerModel<ImageDisplayDataModel>("display");
    static_cast<QtNodes::DataModelRegistry*>(registry.get())->registerModel<PreviewImageDisplayDataModel>("display");

    return registry;
}


#endif //GUI_GIEDATAMODELREGISTRY_H
