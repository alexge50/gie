//
// Created by alex on 5/19/19.
//

#include "GieDataModelRegistry.h"

#include <nodes/NodeDataModel>
#include "GieNodeDataModel.h"

void GieDataModelRegistry::registerModel(const Symbol& symbol, const QString& category)
{
    RegistryItemCreator creator = [this, symbol](){ return std::make_unique<GieNodeDataModel>(symbol, m_program.nodeTypeManager().nodeType(symbol)); };

    const QString name = QString::fromStdString(symbol.qualifiedName);

    /*
     * the members that were required to be accessed are private, but they have const qualified accessors
     * since the current function is not const qualified, the const-ness of the results can be safely removed
     * (const-ness can be casted away, as long as the original source wasn't const in the first place,
     * which is confirmed by the non const qualification if this function)
     */
    auto& registeredItemCreators = const_cast<RegisteredModelCreatorsMap&>(registeredModelCreators());
    auto& categories = const_cast<CategoriesSet&>(this->categories());
    auto& registeredModelsCategory = const_cast<RegisteredModelsCategoryMap&>(registeredModelsCategoryAssociation());
    if (registeredItemCreators.count(name) == 0)
    {
        registeredItemCreators[name] = std::move(creator);
        categories.insert(category);
        registeredModelsCategory[name] = category;
    }
}
