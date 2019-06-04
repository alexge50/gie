//
// Created by alex on 5/18/19.
//

#include <utility>
#include <memory>

#include <gie/NodeLogic.h>

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


Editor::Editor(Program& program, QWidget* parent): QWidget(parent), m_program{program}
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

    vlayout->addWidget(m_noProjectMessage = new QLabel("No project loaded. Consider loading a project: Files > Open Project"));

    connect(m_scene, &QtNodes::FlowScene::nodePlaced, this, &Editor::sceneChanged);
    connect(m_scene, &QtNodes::FlowScene::nodeDeleted, this, &Editor::sceneChanged);
    connect(m_scene, &QtNodes::FlowScene::connectionCreated, this, &Editor::sceneChanged);
    connect(m_scene, &QtNodes::FlowScene::connectionDeleted, this, &Editor::sceneChanged);
    connect(m_scene, &QtNodes::FlowScene::nodeMoved, this, &Editor::sceneChanged);
    connect(m_scene, &QtNodes::FlowScene::nodeDoubleClicked, this, &Editor::sceneChanged);
    connect(m_scene, &QtNodes::FlowScene::connectionHovered, this, &Editor::sceneChanged);
}

void Editor::onConnectionCreated(const QtNodes::Connection& c)
{
    auto giver = dynamic_cast<GieNodeDataModel*>(c.getNode(QtNodes::PortType::Out)->nodeDataModel());
    auto receiver = dynamic_cast<GieNodeDataModel*>(c.getNode(QtNodes::PortType::In)->nodeDataModel());

    if(giver != nullptr && receiver != nullptr)
    {
        auto portIndex = c.getPortIndex(QtNodes::PortType::In);

        auto node = m_program.getNode(receiver->nodeId());
        node.m_logic.m_argument[portIndex] = giver->nodeId();

        m_program.editNode(receiver->nodeId(), node);

        std::cout << "onConnectionCreated: connected " << giver->nodeId() << " with " << receiver->nodeId() << std::endl;
    }
    if(giver != nullptr)
    {
        if(auto receiver = dynamic_cast<TargetExportImageDataModel*>(c.getNode(QtNodes::PortType::In)->nodeDataModel()); receiver != nullptr)
        {
            m_targets.insert({receiver->getId(), receiver->getTargetName()});
            m_program.addResult(receiver->getId().toUtf8().constData(), giver->nodeId());
        }
    }
}

void Editor::onConnectionDeleted(const QtNodes::Connection& c)
{
    if(auto receiver = dynamic_cast<GieNodeDataModel*>(c.getNode(QtNodes::PortType::In)->nodeDataModel()); receiver != nullptr)
    {
        auto portIndex = c.getPortIndex(QtNodes::PortType::In);

        auto node = m_program.getNode(receiver->nodeId());
        node.m_logic.m_argument[portIndex] = NoArgument{};

        m_program.editNode(receiver->nodeId(), node);

        std::cout << "onConnectionRemoved: removed argument from " << receiver->nodeId() << std::endl;
    }
    else if(auto receiver = dynamic_cast<TargetExportImageDataModel*>(c.getNode(QtNodes::PortType::In)->nodeDataModel()); receiver != nullptr)
    {
        m_targets.erase(receiver->getId());
        m_program.removeResult(receiver->getId().toUtf8().constData());
    }
}

void Editor::nodeCreated(QtNodes::Node& node)
{
    if(auto* p = dynamic_cast<TargetExportImageDataModel*>(node.nodeDataModel()); p != nullptr)
    {
        connect(p, &TargetExportImageDataModel::targetNameChanged, this, &Editor::onTargetNameChanged);
        Q_EMIT(attachDockWindow(p->dockWidget()));
    }
}

void Editor::nodeDeleted(QtNodes::Node& node)
{
    if(auto* p = dynamic_cast<TargetExportImageDataModel*>(node.nodeDataModel()); p != nullptr)
    {
        Q_EMIT(detachDockWindow(p->dockWidget()));
    }
}

void Editor::onTargetNameChanged(const QUuid& id, const QString& name)
{
    if(auto it = m_targets.find(id); it != m_targets.end())
        it->second = name;
}

void Editor::addImageNode(const ProjectImage& projectImage)
{
    m_scene->createNode(std::make_unique<ManagedImageSourceDataModel>(projectImage));
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
    auto results = m_program.run();
    std::string id = uuid.toString().toUtf8().constData();

    auto it = std::find_if(results.begin(), results.end(), [&id](const auto& x)
    {
        return x.tag == id;
    });

    if(it != results.end())
    {
        auto image = boost::python::extract<Image>(it->value.m_object)();

        auto imageData = new uint8_t[image.width * image.height * 3];
        std::memcpy(imageData, image.raw(), image.width * image.height * 3);

        QImage qImage(imageData, image.width, image.height, QImage::Format_RGB888, [](auto p){ delete static_cast<uint8_t*>(p); });
        qImage.save(filename);
    }
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