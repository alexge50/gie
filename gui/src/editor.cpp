#include <utility>

//
// Created by alex on 5/18/19.
//

#include <gie/NodeLogic.h>

#include "editor.h"
#include <QVBoxLayout>
#include <QKeyEvent>

#include "gie/GieDataModelRegistry.h"
#include "gie/GieNodeDataModel.h"
#include "gie/DisplayNodeDataModel/PreviewImageDisplayDataModel.h"

#include <nodes/Connection>
#include <nodes/Node>
#include <QtWidgets/QFileDialog>

#include "src/serialisation/serialisation.h"

#include "src/newproject/newproject.h"

#include "Project.h"

Editor::Editor(Program& program, QWidget* parent): QWidget(parent), m_program{program}
{
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

    vlayout->addWidget(m_noProjectMessage = new QLabel("No project loaded. Consider loading a project: Files > Open Project"));
}

void Editor::onConnectionCreated(const QtNodes::Connection& c)
{
    auto giver = dynamic_cast<GieNodeDataModel*>(c.getNode(QtNodes::PortType::Out)->nodeDataModel());
    auto receiver = dynamic_cast<GieNodeDataModel*>(c.getNode(QtNodes::PortType::In)->nodeDataModel());

    if(giver != nullptr && receiver != nullptr)
    {
        auto portIndex = c.getPortIndex(QtNodes::PortType::Out);

        auto node = m_program.getNode(receiver->nodeId());
        node.m_logic.m_argument[portIndex] = giver->nodeId();

        m_program.editNode(receiver->nodeId(), node);

        std::cout << "onConnectionCreated: connected " << giver->nodeId() << " with " << receiver->nodeId() << std::endl;
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
}

void Editor::nodeCreated(QtNodes::Node &n)
{
    if(auto* p = dynamic_cast<PreviewImageDisplayDataModel*>(n.nodeDataModel()); p != nullptr)
        Q_EMIT(attachDockWindow(p->dockWidget()));
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
}

void Editor::keyPressEvent(QKeyEvent* e)
{
    if(e->key() == Qt::Key_S && (e->modifiers().testFlag(Qt::ControlModifier)))
        m_project->save();
}

void Editor::setRegistry(std::shared_ptr<QtNodes::DataModelRegistry> registry)
{
    m_scene->setRegistry(std::move(registry));
}