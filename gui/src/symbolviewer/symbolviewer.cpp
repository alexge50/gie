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
    m_treeWidget->clear();

    QList<QTreeWidgetItem*> list;

    for(const auto& [name, functions]: modules)
    {
        auto entry = new QTreeWidgetItem(QStringList() << name);

        for(const auto& function: functions)
            entry->addChild(new QTreeWidgetItem(QStringList() << function));

        list.append(entry);
    }

    m_treeWidget->addTopLevelItems(list);
}