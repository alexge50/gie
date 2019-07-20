//
// Created by alex on 5/23/19.
//

#ifndef GUI_SYMBOLVIEWER_H
#define GUI_SYMBOLVIEWER_H

#include <QWidget>
#include <QTreeWidget>

#include <set>
#include <map>

class SymbolViewer: public QWidget
{
    Q_OBJECT
public:
    SymbolViewer(QWidget* parent = nullptr);

public Q_SLOTS:
    void onSymbolsUpdate(const std::map<QString, std::vector<QString>>&);
    void removeSymbol(const QString& category, const QString& name);

private:
    QTreeWidget* m_treeWidget;
    std::map<QString, QTreeWidgetItem*> m_modules;
    std::map<QString, std::set<QString>> m_symbols;
};


#endif //GUI_SYMBOLVIEWER_H
