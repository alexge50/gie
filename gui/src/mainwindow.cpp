//
// Created by alex on 5/18/19.
//

#include <QtWidgets/QVBoxLayout>
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "editor.h"
#include <QDockWidget>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    qRegisterMetaType<std::vector<GieSymbol>>("std::vector<GieSymbol>");

    ui->setupUi(this);

    setCentralWidget(m_editor = new Editor());

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
            m_editor, &Editor::detachDockWindow,
            this, &MainWindow::onDetachDockWindow
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

    QObject::connect(
            m_editor, &Editor::reloadedSymbols,
            this, &MainWindow::reloadedSymbols
    );


    QDockWidget* projectScriptsDock = new QDockWidget("Project Scripts", this);
    m_projectScripts = new ProjectScripts{};

    projectScriptsDock->setWidget(m_projectScripts);
    addDockWidget(Qt::RightDockWidgetArea, projectScriptsDock);

    connect(m_editor, &Editor::scriptAdded, m_projectScripts, &ProjectScripts::addFile);
    connect(m_editor, &Editor::scriptRemoved, m_projectScripts, &ProjectScripts::removeFile);
    connect(m_editor, &Editor::projectLoaded, [this](const Project& project)
    {
        m_projectScripts->setScriptsFolder(project.projectPath().filePath("scripts"));
    });

    QFile file("config");
    file.open(QIODevice::ReadOnly);

    QJsonObject config = QJsonDocument::fromJson(file.readAll()).object();

    QString rootPath = QFileInfo(file).absoluteDir().absolutePath();
    for(const auto& module: config["modules"].toArray())
    {
        QString name = module.toObject()["name"].toString();
        QString path = rootPath + "/" + module.toObject()["path"].toString();

        m_editor->loadModule(name, path);
    }


    this->setWindowTitle("GIE");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::reloadedSymbols(const std::vector<GieSymbol>& gieSymbols)
{
    std::map<QString, std::vector<QString>> symbols;

    for(const auto& symbol: gieSymbols)
        symbols[QString::fromStdString(symbol.symbol.module)].push_back(
                QString::fromStdString(symbol.symbol.prettyName)
                );

    Q_EMIT onSymbolsImported(symbols);
}

void MainWindow::onAttachDockWindow(QDockWidget* dock)
{
    dock->setParent(this);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
}

void MainWindow::onDetachDockWindow(QDockWidget* dock)
{
    removeDockWidget(dock);
}

void MainWindow::onChanged()
{
    this->setWindowTitle("GIE [ " + m_editor->getProjectName() + " ]*");
}

void MainWindow::onSaved()
{
    this->setWindowTitle("GIE [ " + m_editor->getProjectName() + " ]");
}
