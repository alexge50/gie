//
// Created by alex on 5/18/19.
//

#include <QtWidgets/QVBoxLayout>
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "editor.h"
#include <QDockWidget>
#include "colorpicker/colorpicker.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

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

    QFile file("config");
    file.open(QIODevice::ReadOnly);

    QJsonObject config = QJsonDocument::fromJson(file.readAll()).object();
    
    for(const auto& module: config["modules"].toArray())
        m_program.import(module.toString().toUtf8().constData());

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
            ui->actionImportImage, &QAction::triggered,
            m_editor, &Editor::onImportImage
    );

    QObject::connect(
            m_editor, &Editor::attachDockWindow,
            this, &MainWindow::onAttachDockWindow
    );

    QObject::connect(
            m_editor, &Editor::sceneChanged,
            this, &MainWindow::onChanged
    );

    QObject::connect(
            m_editor, &Editor::savedProject,
            this, &MainWindow::onSaved
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

    QDockWidget* imageViewerDock = new QDockWidget("ImportedImages", this);
    m_imageViewer = new ImportedImagesViewer(imageViewerDock);

    imageViewerDock->setWidget(m_imageViewer);
    addDockWidget(Qt::LeftDockWidgetArea, imageViewerDock);

    QObject::connect(
            m_editor, &Editor::importedImagesReload,
            m_imageViewer, &ImportedImagesViewer::onImagesUpdate
    );

    QObject::connect(
            m_imageViewer, &ImportedImagesViewer::imagePressed,
            m_editor, &Editor::addImageNode
    );

    QObject::connect(
            ui->actionExportImage, &QAction::triggered,
            m_editor, &Editor::onExportImage
    );

    this->setWindowTitle("GIE");
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

void MainWindow::onChanged()
{
    this->setWindowTitle("GIE [ " + m_editor->getProjectName() + " ]*");
}

void MainWindow::onSaved()
{
    this->setWindowTitle("GIE [ " + m_editor->getProjectName() + " ]");
}
