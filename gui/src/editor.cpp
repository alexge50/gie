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

    connect(m_nodeEditor, &NodeEditor::nodeCreated, this, &Editor::nodeCreated);
    connect(m_nodeEditor, &NodeEditor::nodeDeleted, this, &Editor::nodeDeleted);
    connect(m_nodeEditor, &NodeEditor::argumentRemoved, this, &Editor::argumentRemoved);
    connect(m_nodeEditor, &NodeEditor::argumentEdited, this, &Editor::argumentEdited);
    connect(m_nodeEditor, &NodeEditor::sourceDataChanged, this, &Editor::sourceDataChanged);

    connect(m_nodeEditor, &NodeEditor::sceneChanged, this, &Editor::sceneChanged);

    //auto thread = new QThread;
    m_gie = new Gie{};

    //m_gie->moveToThread(thread);
    //connect(m_gie, &Gie::finished, thread, &QThread::quit);
    //connect(m_gie, &Gie::finished, m_gie, &QThread::deleteLater);
    //connect(thread, &QThread::finished, thread, &QThread::deleteLater);

    //thread->start();

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

    connect(m_gie, &Gie::symbolsAdded, this, &Editor::reloadNode);

    connect(m_gie, &Gie::resultUpdated, this, &Editor::resultUpdated);

    connect(m_gie, &Gie::runtimeError, this, &Editor::runtimeError);

    m_projectScriptsWatcher = new QFileSystemWatcher{this};
    connect(this, &Editor::projectLoaded, [this](const Project& project)
    {
        for(const auto& file: m_scripts)
            removeScript(file.first);
        m_scripts.clear();

        for(const auto& dir: m_projectScriptsWatcher->directories())
            QMetaObject::invokeMethod(
                    m_gie,
                    "removeModulesDirectory",
                    Q_ARG(std::string, dir.toStdString())
                    );

        m_projectScriptsWatcher->removePaths(m_projectScriptsWatcher->directories());
        m_projectScriptsWatcher->removePaths(m_projectScriptsWatcher->files());

        auto path = project.projectPath().filePath("scripts");
        m_projectScriptsWatcher->addPath(path);
        QMetaObject::invokeMethod(
                m_gie,
                "addModulesDirectory",
                Q_ARG(std::string, path.toStdString())
        );

        for(const auto& file: QDir{path}.entryInfoList(QStringList{"*.py"}))
        {
            addScript(file.canonicalFilePath());
            m_scripts[file.canonicalFilePath()] = file.lastModified();
        }
    });

    connect(m_projectScriptsWatcher, &QFileSystemWatcher::directoryChanged, [this](const auto& path)
    {
        std::map<QString, QDateTime> scripts;

        for(const auto& file: QDir{path}.entryInfoList(QStringList{"*.py"}))
            scripts[file.canonicalFilePath()] = file.lastModified();

        for(const auto& file: m_scripts)
        {
            if(scripts.find(file.first) == scripts.end())
                removeScript(file.first);
        }

        for(const auto& file: scripts)
        {
            if(m_scripts.find(file.first) == m_scripts.end() || m_scripts[file.first] != file.second)
                addScript(file.first);
        }

        m_scripts = std::move(scripts);
    });


    connect(this, &Editor::beforeProjectLoading, [this]{
        m_nodeEditor->getRegistry()->removeCreatorIf(
                [this](const auto& p)
                {
                    if(p.first.startsWith("user."))
                    {
                        QString category = p.first.left(p.first.lastIndexOf('.'));
                        QString name = p.first.mid(p.first.lastIndexOf('.') + 1);

                        Q_EMIT symbolRemoved(category, name);

                        return true;
                    }
                    return false;
                });
    });
}

void Editor::resultUpdated(QUuid displayId, Data data)
{
    if(auto* display = dynamic_cast<DisplayNode*>(
                m_nodeEditor->scene()->nodes().at(displayId)->nodeDataModel()
        ); display != nullptr)
        display->displayData(std::move(data));
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
    Q_EMIT beforeProjectLoading();

    m_project = std::make_unique<Project>(newProject(directory, std::move(name), *(m_nodeEditor->scene())));
    m_noProjectMessage->hide();
    m_nodeEditor->show();

    reloadImages();
    Q_EMIT savedProject();
    Q_EMIT projectLoaded(*m_project);
}

void Editor::onOpenProject()
{
    auto directory = QFileDialog::getExistingDirectory(
            this,
            tr("Open gie project"),
            QDir::homePath()
    );

    Q_EMIT beforeProjectLoading();

    m_project = std::make_unique<Project>(loadProject(directory, *this, *(m_nodeEditor->scene())));

    m_noProjectMessage->hide();
    m_nodeEditor->show();

    reloadImages();
    Q_EMIT savedProject();
    Q_EMIT projectLoaded(*m_project);
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

void Editor::addScript(const QString& path)
{
    QString name = QFileInfo{path}.baseName();

    QMetaObject::invokeMethod(
            m_gie,
            "loadModule",
            Q_ARG(std::string, "user." + name.toStdString()),
            Q_ARG(std::string, path.toStdString())
    );

    Q_EMIT scriptAdded(QFileInfo{path}.fileName());
}

void Editor::removeScript(const QString& path)
{
    Q_EMIT scriptRemoved(QFileInfo{path}.fileName());

}

void Editor::reloadNode(const std::vector<GieSymbol>& symbols)
{
    for (const auto &symbol: symbols) {
        m_nodeEditor->registerNodeType([symbol] {
            return std::make_unique<GieNode>(symbol);
        }, QString::fromStdString(symbol.symbol.module));

        auto toEdit = m_nodes[symbol.symbol.qualifiedName];
        for (auto nodeId: toEdit) {
            if (m_nodeEditor->scene()->nodes().find(nodeId) == m_nodeEditor->scene()->nodes().end())
                continue;

            const auto &node = m_nodeEditor->scene()->nodes().at(nodeId);

            if (auto *gieNode = dynamic_cast<GieNode *>(
                        node->nodeDataModel()
                ); gieNode != nullptr) {
                if ([&] {
                    if (gieNode->m_returnType != symbol.returnType)
                        return true;

                    if (gieNode->m_arguments.size() != symbol.arguments.size())
                        return true;

                    for (std::size_t i = 0; i < gieNode->m_arguments.size(); i++) {
                        if (gieNode->m_arguments[i].m_argumentType != symbol.arguments[i].m_argumentType)
                            return true;
                        if (gieNode->m_arguments[i].m_argumentName != symbol.arguments[i].m_argumentName)
                            return true;
                    }

                    return false;
                }()) {
                    auto position = m_nodeEditor->scene()->getNodePosition(*node);
                    m_nodeEditor->scene()->removeNode(node.get());

                    auto &newNode = m_nodeEditor->scene()->createNode(std::make_unique<GieNode>(symbol));
                    m_nodeEditor->scene()->setNodePosition(newNode, position);
                }
                else
                {
                    gieNode->m_arguments = symbol.arguments;
                    gieNode->ok();
                }
            }
        }
    }
}

void Editor::nodeCreated(BaseNode* node)
{
    if(auto* p = dynamic_cast<GieNode*>(node); p != nullptr)
    {
        m_nodes[p->symbol().qualifiedName].insert(p->id());
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
}

void Editor::nodeDeleted(BaseNode* node)
{
    if(auto* p = dynamic_cast<GieNode*>(node); p != nullptr)
    {
        m_nodes[p->symbol().qualifiedName].erase(p->id());
        QMetaObject::invokeMethod(
                m_gie,
                "removeNode",
                Q_ARG(GieNodeId, GieNodeId{node->id()})
        );
    }

    if(auto* p = dynamic_cast<TargetExportImageNode*>(node); p != nullptr)
    {
        Q_EMIT(detachDockWindow(p->dockWidget()));
    }
}

void Editor::argumentRemoved(BaseNode* node, std::size_t port)
{
    QMetaObject::invokeMethod(
            m_gie,
            "removeArgument",
            Q_ARG(GieNodeId, GieNodeId{node->id()}),
            Q_ARG(ArgumentId, ArgumentId{port})
    );

    if(auto* p = dynamic_cast<DisplayNode*>(node); p != nullptr)
    {
        m_toUpdate.erase(node->id());
        m_displaysToUpdate.erase(node->id());

        QMetaObject::invokeMethod(
                m_gie,
                "removeResultNotify",
                Q_ARG(QUuid, p->id())
        );
    }

    if(auto* p = dynamic_cast<TargetExportImageNode*>(node); p != nullptr)
        m_targets.erase(p->id());
}

void Editor::argumentEdited(BaseNode* node, QUuid argumentId, std::size_t port)
{
    if(auto* p = dynamic_cast<GieNode*>(node); p != nullptr)
    {

        if(auto* other = dynamic_cast<SourceNode*>(
                    m_nodeEditor->scene()->nodes().at(argumentId)->nodeDataModel()
            ); other != nullptr)
        {
            m_toUpdate[argumentId].insert({node->id(), port});

            QMetaObject::invokeMethod(
                    m_gie,
                    "editNode",
                    Q_ARG(GieNodeId, GieNodeId{node->id()}),
                    Q_ARG(ArgumentId, ArgumentId{port}),
                    Q_ARG(Data, other->getData())
            );
        }
        else
        {
            QMetaObject::invokeMethod(
                    m_gie,
                    "editNode",
                    Q_ARG(GieNodeId, GieNodeId{node->id()}),
                    Q_ARG(ArgumentId, ArgumentId{port}),
                    Q_ARG(GieNodeId, GieNodeId{argumentId})
            );
        }
    }

    if(auto* p = dynamic_cast<DisplayNode*>(node); p != nullptr)
    {
        if(auto* other = dynamic_cast<SourceNode*>(
                    m_nodeEditor->scene()->nodes().at(argumentId)->nodeDataModel()
            ); other != nullptr)
        {
            m_displaysToUpdate[argumentId].insert(p->id());
        }
        else if(auto* gieNode = dynamic_cast<GieNode*>(
                    m_nodeEditor->scene()->nodes().at(argumentId)->nodeDataModel()
            ); gieNode != nullptr)
        {
            QMetaObject::invokeMethod(
                    m_gie,
                    "addResultNotify",
                    Q_ARG(QUuid, p->id()),
                    Q_ARG(GieNodeId, GieNodeId{gieNode->id()})
            );
        }
    }

    if(auto* p = dynamic_cast<TargetExportImageNode*>(node); p != nullptr)
        m_targets.insert({p->id(), p->getTargetName()});
}

void Editor::sourceDataChanged(QUuid nodeId, Data data)
{
    m_values[nodeId] = std::move(data);

    for(auto toUpdate: m_toUpdate[nodeId])
    {
        QMetaObject::invokeMethod(
                m_gie,
                "editNode",
                Q_ARG(GieNodeId, GieNodeId{toUpdate.id}),
                Q_ARG(ArgumentId, ArgumentId{static_cast<std::size_t>(toUpdate.port)}),
                Q_ARG(Data, m_values[nodeId])
        );
    }

    for(auto displayId: m_displaysToUpdate[nodeId])
    {
        if(auto* display = dynamic_cast<DisplayNode*>(
                    m_nodeEditor->scene()->nodes().at(displayId)->nodeDataModel()
            ); display != nullptr)
            display->displayData(m_values[nodeId]);
    }
}

void Editor::runtimeError(const GieRuntimeError& error)
{
    if(error.nodeId)
        m_nodeEditor->errorNode(*error.nodeId, "internal python error");
    Q_EMIT this->error(QString::fromStdString(error.errorMessage));
}