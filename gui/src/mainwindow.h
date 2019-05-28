//
// Created by alex on 5/18/19.
//

#ifndef GUI_MAINWINDOW_H
#define GUI_MAINWINDOW_H

#include <gie/Program.h>

#include <QMainWindow>
#include <QMenuBar>
#include "editor.h"
#include "symbolviewer/symbolviewer.h"
#include "importedimagesviewer/importedimagesviewer.h"
#include "src/gie/GieDataModelRegistry.h"

#include "src/ScopedGILRelease.h"

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

private:
    void reloadSymbols();

private:
    Ui::MainWindow *ui;
    Program m_program;
    QMenuBar* m_menu;
    Editor* m_editor;

    SymbolViewer* m_symbolViewer;
    ImportedImagesViewer* m_imageViewer;
    std::shared_ptr<GieDataModelRegistry> m_modelRegistry;
};

#endif //GUI_MAINWINDOW_H
