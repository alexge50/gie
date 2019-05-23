//
// Created by alex on 5/23/19.
//

#ifndef GUI_SYMBOLVIEWER_H
#define GUI_SYMBOLVIEWER_H

#include <QWidget>
#include <QTreeWidget>

class SymbolViewer: public QWidget
{
    Q_OBJECT
public:
    SymbolViewer(QWidget* parent = nullptr);

public Q_SLOTS:
    void onSymbolsUpdate(const std::map<std::string, std::vector<std::string>>&);

private:
    QTreeWidget* m_treeWidget;
};


#endif //GUI_SYMBOLVIEWER_H
