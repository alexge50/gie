//
// Created by alex on 5/18/19.
//

#include <utility>
#include <memory>

#include <gie/Node.h>

#include "editor.h"
#include <QVBoxLayout>
#include <QKeyEvent>

#include "gie/GieDataModelRegistry.h"
#include "gie/GieNodeDataModel.h"
#include "gie/SourceNodeDataModel/ManagedImageSourceDataModel.h"

#include <nodes/Connection>
#include <nodes/Node>
#include <nodes/FlowViewStyle>
#include <nodes/ConnectionStyle>
#include <nodes/NodeStyle>
#include <QtWidgets/QFileDialog>

#include "src/serialisation/serialisation.h"

#include "src/newproject/newproject.h"
#include "src/importimage/importimage.h"
#include "src/exportimage/exportimage.h"

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

    m_scene = new QtNodes::FlowScene();

    m_view = new QtNodes::FlowView(m_scene);
    m_view->setSceneRect(-640000, -640000, 640000, 640000);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->hide();

    vlayout->addWidget(m_view);

    QObject::connect(
        m_scene, &QtNodes::FlowScene::connectionCreated,
        this, &Editor::onConnectionCreated
    );

    QObject::connect(
            m_scene, &QtNodes::FlowScene::connectionDeleted,
            this, &Editor::onConnectionDeleted
    );

    QObject::connect(
            m_scene, &QtNodes::FlowScene::nodeCreated,
            this, &Editor::nodeCreated
    );

    QObject::connect(
            m_scene, &QtNodes::FlowScene::nodeDeleted,
            this, &Editor::nodeDeleted
    );

    vlayout->addWidget(m_noProjectMessage = new QLabel("No project loaded.\nNew Project: File > New Project\nOpen Project: File > Open Project"));

    connect(m_scene, &QtNodes::FlowScene::nodePlaced, this, &Editor::sceneChanged);
    connect(m_scene, &QtNodes::FlowScene::nodeDeleted, this, &Editor::sceneChanged);
    connect(m_scene, &QtNodes::FlowScene::connectionCreated, this, &Editor::sceneChanged);
    connect(m_scene, &QtNodes::FlowScene::connectionDeleted, this, &Editor::sceneChanged);
    connect(m_scene, &QtNodes::FlowScene::nodeMoved, this, &Editor::sceneChanged);
    connect(m_scene, &QtNodes::FlowScene::nodeDoubleClicked, this, &Editor::sceneChanged);
    connect(m_scene, &QtNodes::FlowScene::connectionHovered, this, &Editor::sceneChanged);

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

    connect(m_gie, &Gie::reloadedSymbols, this, &Editor::reloadedSymbols);
    connect(m_gie, &Gie::resultUpdated, this, &Editor::resultUpdated);
}

void Editor::onConnectionCreated(const QtNodes::Connection& c)
{
    auto giver = dynamic_cast<GieNodeDataModel*>(c.getNode(QtNodes::PortType::Out)->nodeDataModel());
    auto receiver = dynamic_cast<GieNodeDataModel*>(c.getNode(QtNodes::PortType::In)->nodeDataModel());

    if(giver != nullptr && receiver != nullptr)
    {
        [[maybe_unused]]auto portIndex = static_cast<std::size_t>(c.getPortIndex(QtNodes::PortType::In));

        QMetaObject::invokeMethod(
                m_gie,
                "editNode",
                Q_ARG(GieNodeId, receiver->m_nodeId),
                Q_ARG(ArgumentId, ArgumentId{portIndex}),
                Q_ARG(GieNodeId, giver->m_nodeId)
                );
    }

    if(giver != nullptr)
    {
        if(auto receiver = dynamic_cast<TargetExportImageDataModel*>(c.getNode(QtNodes::PortType::In)->nodeDataModel()); receiver != nullptr)
        {
            m_targets.insert({receiver->getId(), receiver->getTargetName()});
        }
    }

    if(auto receiver = dynamic_cast<GieDisplayDataModel*>(c.getNode(QtNodes::PortType::In)->nodeDataModel()); receiver != nullptr)
    {
        if(giver != nullptr)
        {
            QMetaObject::invokeMethod(
                    m_gie,
                    "addResultNotify",
                    Q_ARG(QUuid, c.getNode(QtNodes::PortType::In)->id()),
                    Q_ARG(GieNodeId, giver->m_nodeId)
            );
        }
    }
}

void Editor::onConnectionDeleted(const QtNodes::Connection& c)
{
    if(auto receiver = dynamic_cast<GieNodeDataModel*>(c.getNode(QtNodes::PortType::In)->nodeDataModel()); receiver != nullptr)
    {
        [[maybe_unused]]auto portIndex = static_cast<std::size_t>(c.getPortIndex(QtNodes::PortType::In));

        QMetaObject::invokeMethod(
                m_gie,
                "removeArgument",
                Q_ARG(GieNodeId, receiver->m_nodeId),
                Q_ARG(ArgumentId, ArgumentId{portIndex})
                );
    }

    if(auto receiver = dynamic_cast<TargetExportImageDataModel*>(c.getNode(QtNodes::PortType::In)->nodeDataModel()); receiver != nullptr)
    {
        m_targets.erase(receiver->getId());
    }

    if(auto receiver = dynamic_cast<GieDisplayDataModel*>(c.getNode(QtNodes::PortType::In)->nodeDataModel()); receiver != nullptr)
    {
        QMetaObject::invokeMethod(
                m_gie,
                "removeResultNotify",
                Q_ARG(QUuid, c.getNode(QtNodes::PortType::In)->id())
                );
    }

}

void Editor::nodeCreated(QtNodes::Node& node)
{
    if(auto* p = dynamic_cast<TargetExportImageDataModel*>(node.nodeDataModel()); p != nullptr)
    {
        connect(p, &TargetExportImageDataModel::targetNameChanged, this, &Editor::onTargetNameChanged);
        Q_EMIT(attachDockWindow(p->dockWidget()));
    }

    if(auto* p = dynamic_cast<GieNodeDataModel*>(node.nodeDataModel()); p != nullptr)
    {
        p->m_nodeId = node.id();

        QMetaObject::invokeMethod(
                m_gie,
                "addNode",
                Q_ARG(GieNodeId, p->m_nodeId),
                Q_ARG(std::string, p->m_symbol.qualifiedName)
                );

        connect(p, &GieNodeDataModel::edit, [this, p](QUuid value, int port)
        {
            QMetaObject::invokeMethod(
                    m_gie,
                    "editNode",
                    Q_ARG(GieNodeId, p->m_nodeId),
                    Q_ARG(ArgumentId, ArgumentId{static_cast<std::size_t>(port)}),
                    Q_ARG(Data, m_values[value])
            );
        });

    }

    if(auto* p = dynamic_cast<GieSourceDataModel*>(node.nodeDataModel()); p != nullptr)
    {
        connect(p, &GieSourceDataModel::valueChanged, [this, &node](Data data)
        {
            m_values[node.id()] = std::move(data);
        });

        p->m_valueId = node.id();
    }

    if(auto* p = dynamic_cast<ManagedImageSourceDataModel*>(node.nodeDataModel()); p != nullptr)
    {
        m_values[node.id()] = p->m_image;
        p->m_valueId = node.id();
    }

    if(auto* p = dynamic_cast<GieDisplayDataModel*>(node.nodeDataModel()); p != nullptr)
    {

    }
}

void Editor::nodeDeleted(QtNodes::Node& node)
{
    if(auto* p = dynamic_cast<TargetExportImageDataModel*>(node.nodeDataModel()); p != nullptr)
    {
        Q_EMIT(detachDockWindow(p->dockWidget()));
    }

    if(auto* p = dynamic_cast<GieNodeDataModel*>(node.nodeDataModel()); p != nullptr)
    {
        QMetaObject::invokeMethod(
                m_gie,
                "removeNode",
                Q_ARG(GieNodeId, p->m_nodeId)
                );
    }

    if(auto* p = dynamic_cast<GieSourceDataModel*>(node.nodeDataModel()); p != nullptr)
    {
        m_values.erase(node.id());
    }
}

void Editor::resultUpdated(QUuid displayerId, Data data)
{
    dynamic_cast<GieDisplayDataModel*>(m_scene->nodes().at(displayerId)->nodeDataModel())->displayData(std::move(data));
}

void Editor::onTargetNameChanged(const QUuid& id, const QString& name)
{
    if(auto it = m_targets.find(id); it != m_targets.end())
        it->second = name;
}

void Editor::addImageNode(const ProjectImage& projectImage)
{
    auto& node = m_scene->createNode(std::make_unique<ManagedImageSourceDataModel>(projectImage));
    m_values[node.id()] = dynamic_cast<ManagedImageSourceDataModel*>(node.nodeDataModel())->m_image;
}

void Editor::onNewProject()
{
    auto dialog = new NewProject();
    connect(dialog, &NewProject::newProject, this, &Editor::onNewProject_);
    dialog->show();
}

void Editor::onNewProject_(QDir directory, QString name)
{
    m_project = std::make_unique<Project>(newProject(directory, std::move(name), *m_scene));
    m_noProjectMessage->hide();
    m_view->show();

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

    m_project = std::make_unique<Project>(loadProject(directory, *m_scene));

    m_noProjectMessage->hide();
    m_view->show();

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
}

void Editor::setRegistry(std::shared_ptr<QtNodes::DataModelRegistry> registry)
{
    m_scene->setRegistry(std::move(registry));
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
