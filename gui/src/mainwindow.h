//
// Created by alex on 5/18/19.
//

#ifndef GUI_MAINWINDOW_H
#define GUI_MAINWINDOW_H

#include <gie/Program.h>

#include <QMainWindow>
#include <QMenuBar>
#include "editor.h"
#include "widgets/symbolviewer/symbolviewer.h"
#include "widgets/importedimagesviewer/importedimagesviewer.h"
#include "widgets/projectscripts/projectscripts.h"
#include "widgets/logconsole/logconsole.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

Q_SIGNALS:
    void onSymbolsImported(const std::map<QString, std::vector<QString>>&);

private Q_SLOTS:
    void onAttachDockWindow(QDockWidget*);
    void onDetachDockWindow(QDockWidget*);
    void onChanged();
    void onSaved();

private Q_SLOTS:
    void reloadedSymbols(const std::vector<GieSymbol>&);

private:
    Ui::MainWindow *ui;
    Editor* m_editor;

    SymbolViewer* m_symbolViewer;
    ImportedImagesViewer* m_imageViewer;
    ProjectScripts* m_projectScripts;
    LogConsole* m_logConsole;
};

#endif //GUI_MAINWINDOW_H
