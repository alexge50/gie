//
// Created by alex on 5/18/19.
//

#include <QtWidgets/QVBoxLayout>
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "editor.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_program.context().module("builtins", false);

    auto sys = m_program.context().module("sys", false);
    auto os = m_program.context().module("os", false);

    sys.attr("path").attr("insert")(1, os.attr("getcwd")());

    m_program.context().module("modules.internals", false);
    m_program.import("modules.arithmetic");
    m_program.import("modules.string");
    m_program.import("modules.colors");

    setCentralWidget(m_editor = new Editor(m_program));

    QObject::connect(
            ui->actionSave, &QAction::triggered,
            m_editor, &Editor::onSave
    );

    QObject::connect(
            ui->actionLoad, &QAction::triggered,
            m_editor, &Editor::onLoad
    );
}

MainWindow::~MainWindow()
{
    delete ui;
}
