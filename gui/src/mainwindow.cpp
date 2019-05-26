//
// Created by alex on 5/18/19.
//

#include <QtWidgets/QVBoxLayout>
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "editor.h"
#include <QDockWidget>
#include "colorpicker/colorpicker.h"

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
    m_program.import("modules.images");

    setCentralWidget(m_editor = new Editor(m_program));

    QObject::connect(
            ui->actionNewProject, &QAction::triggered,
            m_editor, &Editor::onNewProject
    );

    QObject::connect(
            ui->actionOpenProject, &QAction::triggered,
            m_editor, &Editor::onOpenProject
    );

    QObject::connect(
            m_editor, &Editor::attachDockWindow,
            this, &MainWindow::onAttachDockWindow
    );


    QDockWidget* symbolViewerDock = new QDockWidget("SymbolViewer", this);
    m_symbolViewer = new SymbolViewer(symbolViewerDock);

    symbolViewerDock->setWidget(m_symbolViewer);
    addDockWidget(Qt::RightDockWidgetArea, symbolViewerDock);

    QObject::connect(
            this, &MainWindow::onSymbolsImported,
            m_symbolViewer, &SymbolViewer::onSymbolsUpdate
    );

    reloadSymbols();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::reloadSymbols()
{
    m_modelRegistry = registerDataModels(m_program);
    m_editor->setRegistry(m_modelRegistry);

    std::map<QString, std::vector<QString>> symbols;

    for(const auto& [name, category]: m_modelRegistry->registeredModelsCategoryAssociation())
        symbols[category].push_back(name);

    Q_EMIT onSymbolsImported(symbols);
}

void MainWindow::onAttachDockWindow(QDockWidget* dock)
{
    dock->setParent(this);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
}