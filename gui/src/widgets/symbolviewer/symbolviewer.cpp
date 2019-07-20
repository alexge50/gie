//
// Created by alex on 5/23/19.
//

#include "symbolviewer.h"

#include <QGridLayout>

SymbolViewer::SymbolViewer(QWidget* parent):
    QWidget(parent)
{
    auto layout = new QGridLayout(this);
    layout->setContentsMargins(QMargins());

    m_treeWidget = new QTreeWidget{};
    m_treeWidget->setHeaderHidden(true);

    layout->addWidget(m_treeWidget, 0, 0);
}

void SymbolViewer::onSymbolsUpdate(const std::map<QString, std::vector<QString>>& modules)
{
    QList<QTreeWidgetItem*> list;

    for(const auto& [name, functions]: modules)
    {
        if(!m_modules.count(name))
        {
            auto entry = new QTreeWidgetItem(QStringList() << name);
            m_modules.insert({name, entry});

            list.append(entry);
        }

        auto entry = m_modules[name];
        for(const auto& function: functions)
        {
            if(!m_symbols[name].count(function))
            {
                entry->addChild(new QTreeWidgetItem(QStringList() << function));
                m_symbols[name].insert(function);
            }
        }

    }

    m_treeWidget->addTopLevelItems(list);
}

void SymbolViewer::removeSymbol(const QString& category, const QString& name)
{
    if(auto it = m_modules.find(category); it != m_modules.end())
    {
        auto entry = it->second;

        for(int i = 0; i < entry->childCount(); i++)
        {
            if(name == entry->child(i)->text(0))
                entry->removeChild(entry->child(i));
        }
        m_symbols[category].erase(name);

        if(m_symbols[category].empty())
        {
            delete m_treeWidget->takeTopLevelItem(m_treeWidget->indexOfTopLevelItem(entry));
            m_modules.erase(category);
        }
    }
}