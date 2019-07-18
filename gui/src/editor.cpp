//
// Created by alex on 5/18/19.
//

#include <utility>
#include <memory>

#include <gie/Node.h>

#include "editor.h"
#include <QVBoxLayout>
#include <QKeyEvent>

#include <QtWidgets/QFileDialog>
#include <src/nodes/GieNode.h>

#include <src/nodes/ColorSourceNode.h>
#include <src/nodes/ImageSourceNode.h>
#include <src/nodes/IntegerSourceNode.h>
#include <src/nodes/NumberSourceNode.h>
#include <src/nodes/StringSourceNode.h>
#include <src/nodes/ColorDisplayNode.h>
#include <src/nodes/ImageDisplayNode.h>
#include <src/nodes/IntegerDisplayNode.h>
#include <src/nodes/NumberDisplayNode.h>
#include <src/nodes/StringDisplayNode.h>
#include <src/nodes/TargetExportImageNode.h>
#include <src/nodes/ManagedImageSourceNode.h>

#include "src/serialisation/serialisation.h"

#include "src/widgets/newproject/newproject.h"
#include "src/widgets/importimage/importimage.h"
#include "src/widgets/exportimage/exportimage.h"

#include "Project.h"


Editor::Editor(QWidget* parent): QWidget(parent)
{
    QtNodes::ConnectionStyle::setConnectionStyle(R"({
    "ConnectionStyle": {
      "UseDataDefinedColors": true
    }
})");

    auto vlayout = new QVBoxLayout(this);

    vlayout->setMargin(0);
    vlayout->setSpacing(0);

    m_nodeEditor = new NodeEditor{};

    vlayout->addWidget(m_noProjectMessage = new QLabel("No project loaded.\nNew Project: File > New Project\nOpen Project: File > Open Project"));
    vlayout->addWidget(m_nodeEditor);
    m_nodeEditor->hide();

    m_nodeEditor->registerNodeType<ColorSourceNode>("sources");
    m_nodeEditor->registerNodeType<ImageSourceNode>("sources");
    m_nodeEditor->registerNodeType<IntegerSourceNode>("sources");
    m_nodeEditor->registerNodeType<NumberSourceNode>("sources");
    m_nodeEditor->registerNodeType<StringSourceNode>("sources");
    m_nodeEditor->registerNodeType<ColorDisplayNode>("displays");
    m_nodeEditor->registerNodeType<ImageDisplayNode>("displays");
    m_nodeEditor->registerNodeType<IntegerDisplayNode>("displays");
    m_nodeEditor->registerNodeType<NumberDisplayNode>("displays");
    m_nodeEditor->registerNodeType<StringDisplayNode>("displays");
    m_nodeEditor->registerNodeType<TargetExportImageNode>("displays");

    connect(m_nodeEditor, &NodeEditor::nodeCreated, [this](BaseNode* node){
        if(auto* p = dynamic_cast<GieNode*>(node); p != nullptr)
        {
            QMetaObject::invokeMethod(
                    m_gie,
                    "addNode",
                    Q_ARG(GieNodeId, GieNodeId{node->id()}),
                    Q_ARG(std::string, p->symbol().qualifiedName)
            );
        }

        if(auto* p = dynamic_cast<TargetExportImageNode*>(node); p != nullptr)
        {
            connect(p, &TargetExportImageNode::targetNameChanged, this, &Editor::onTargetNameChanged);
            Q_EMIT(attachDockWindow(p->dockWidget()));
        }
    });

    connect(m_nodeEditor, &NodeEditor::nodeDeleted, [](BaseNode* node){

    });

    connect(m_nodeEditor, &NodeEditor::argumentRemoved, [](BaseNode*, std::size_t port){

    });

    connect(m_nodeEditor, &NodeEditor::argumentEdited, [](BaseNode*, QUuid argumentId, std::size_t port){

    });

    connect(m_nodeEditor, &NodeEditor::sourceDataChanged, [](QUuid, Data){

    });

    auto thread = new QThread;
    m_gie = new Gie{};

    m_gie->moveToThread(thread);
    connect(m_gie, &Gie::finished, thread, &QThread::quit);
    connect(m_gie, &Gie::finished, m_gie, &QThread::deleteLater);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);

    thread->start();

    QMetaObject::invokeMethod(
            m_gie,
            "init"
            );

    connect(m_gie, &Gie::symbolsAdded, this, &Editor::reloadedSymbols);

    connect(m_gie, &Gie::symbolsAdded, [this](const std::vector<GieSymbol>& symbols)
    {
        for(const auto& symbol: symbols)
        {
            m_nodeEditor->registerNodeType([symbol]{
                return std::make_unique<GieNode>(symbol);
            }, QString::fromStdString(symbol.symbol.module));
        }
    });

    connect(m_gie, &Gie::resultUpdated, this, &Editor::resultUpdated);
}

void Editor::resultUpdated(QUuid displayerId, Data data)
{
}

void Editor::onTargetNameChanged(const QUuid& id, const QString& name)
{
    if(auto it = m_targets.find(id); it != m_targets.end())
        it->second = name;
}

void Editor::addImageNode(const ProjectImage& projectImage)
{
    auto& node = m_nodeEditor->scene()->createNode(std::make_unique<ManagedImageSourceNode>(projectImage));
    m_values[node.id()] = dynamic_cast<ManagedImageSourceNode*>(node.nodeDataModel())->m_image;
}

void Editor::onNewProject()
{
    auto dialog = new NewProject();
    connect(dialog, &NewProject::newProject, this, &Editor::onNewProject_);
    dialog->show();
}

void Editor::onNewProject_(QDir directory, QString name)
{
    m_project = std::make_unique<Project>(newProject(directory, std::move(name), *(m_nodeEditor->scene())));
    m_noProjectMessage->hide();
    m_nodeEditor->show();

    reloadImages();
    Q_EMIT savedProject();
}

void Editor::onOpenProject()
{
    auto directory = QFileDialog::getExistingDirectory(
            this,
            tr("Open gie project"),
            QDir::homePath()
    );

    m_project = std::make_unique<Project>(loadProject(directory, *(m_nodeEditor->scene())));

    m_noProjectMessage->hide();
    m_nodeEditor->show();

    reloadImages();
    Q_EMIT savedProject();
}

void Editor::keyPressEvent(QKeyEvent* e)
{
    if(e->key() == Qt::Key_S && (e->modifiers().testFlag(Qt::ControlModifier)))
    {
        m_project->save();
        Q_EMIT savedProject();
    }
}

void Editor::onImportImage()
{
    auto dialog = new ImportImage();
    connect(dialog, &ImportImage::importImage, this, &Editor::onImportImage_);
    dialog->show();
}

void Editor::onImportImage_(QString path)
{
    QDir p{path};
    QString filename = p.dirName();

    m_project->importImage(path, filename);
    reloadImages();
}

void Editor::onExportImage()
{
    auto exportImage = new ExportImage(m_targets);
    connect(exportImage, &ExportImage::exportImage, this, &Editor::onExportImage_);
    exportImage->show();
}

void Editor::onExportImage_(const QUuid& uuid, const QString& filename)
{
    if(auto* p = dynamic_cast<TargetExportImageNode*>(m_nodeEditor->scene()->nodes().at(uuid)->nodeDataModel()); p != nullptr)
        p->getImage().save(filename);
}

void Editor::reloadImages()
{
    std::vector<ProjectImage> images;

    images.reserve(m_project->importedImages().size());
    for(const auto&[uuid, image]: m_project->importedImages())
        images.push_back(image);

    Q_EMIT importedImagesReload(images);
}

QString Editor::getProjectName()
{
    if(m_project)
        return m_project->projectName();
    return QString("");
}

void Editor::loadModule(const QString& name, const QString& path)
{
    QMetaObject::invokeMethod(
            m_gie,
            "loadModule",
            Q_ARG(std::string, name.toStdString()),
            Q_ARG(std::string, path.toStdString())
            );
}
