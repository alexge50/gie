//
// Created by alex on 5/18/19.
//

#ifndef GUI_MAINWINDOW_H
#define GUI_MAINWINDOW_H

#include <gie/Program.h>
#undef B0

#include <QMainWindow>
#include <QMenuBar>
#include "editor.h"
#include "symbolviewer/symbolviewer.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    Program m_program;
    QMenuBar* m_menu;
    Editor* m_editor;

    SymbolViewer* m_symbolViewer;
};

#endif //GUI_MAINWINDOW_H
