//
// Created by alex on 5/19/19.
//

#ifndef GUI_GIEDATAMODELREGISTRY_H
#define GUI_GIEDATAMODELREGISTRY_H

#include <gie/Node.h>
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
#include "DisplayNodeDataModel/TargetExportImageDataModel.h"

#include "src/Gie.h"

class GieDataModelRegistry: public QtNodes::DataModelRegistry
{
public:
    void registerModel(const GieSymbol& symbol);
};

[[maybe_unused]]
static std::shared_ptr<GieDataModelRegistry> registerDataModels(const std::vector<GieSymbol>& symbols)
{
    std::shared_ptr<GieDataModelRegistry> registry(new GieDataModelRegistry{}, [](auto p){
        delete reinterpret_cast<GieDataModelRegistry*>(p);
    });

    for(const auto& symbol: symbols)
        registry->registerModel(symbol);

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
    static_cast<QtNodes::DataModelRegistry*>(registry.get())->registerModel<TargetExportImageDataModel>("display");

    return registry;
}


#endif //GUI_GIEDATAMODELREGISTRY_H
