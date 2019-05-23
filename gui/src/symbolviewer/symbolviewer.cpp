//
// Created by alex on 5/23/19.
//

#include "symbolviewer.h"

SymbolViewer::SymbolViewer(QWidget* parent):
    QWidget(parent),
    m_treeWidget{new QTreeWidget(this)}
{
    m_treeWidget->addTopLevelItem( new QTreeWidgetItem(QStringList() << "available blocks"));
}

void SymbolViewer::onSymbolsUpdate(const std::map<std::string, std::vector<std::string>>& modules)
{
    m_treeWidget->reset();

    QList<QTreeWidgetItem*> list;

    for(const auto& [name, functions]: modules)
    {
        auto entry = new QTreeWidgetItem(QStringList() << QString::fromStdString(name));

        for(const auto& function: functions)
            entry->addChild(new QTreeWidgetItem(QStringList() << QString::fromStdString(function)));

        list.append(entry);
    }

    m_treeWidget->addTopLevelItems(list);
}