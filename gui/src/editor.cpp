//
// Created by alex on 5/18/19.
//

#include <gie/NodeLogic.h>
#undef B0

#include "editor.h"
#include <QVBoxLayout>

#include "gie/GieDataModelRegistry.h"
#include "gie/GieNodeDataModel.h"

#include <nodes/Connection>
#include <nodes/Node>

Editor::Editor(Program& program, QWidget* parent): QWidget(parent), m_program{program}
{
    auto vlayout = new QVBoxLayout(this);

    vlayout->setMargin(0);
    vlayout->setSpacing(0);

    m_scene = new QtNodes::FlowScene(registerDataModels(m_program));

    m_view = new QtNodes::FlowView(m_scene);
    m_view->setSceneRect(-640000, -640000, 640000, 640000);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    vlayout->addWidget(m_view);

    QObject::connect(
        m_scene, &QtNodes::FlowScene::connectionCreated,
        this, &Editor::onConnectionCreated
    );

    QObject::connect(
            m_scene, &QtNodes::FlowScene::connectionDeleted,
            this, &Editor::onConnectionDeleted
    );
}

void Editor::onConnectionCreated(const QtNodes::Connection& c)
{
    auto giver = reinterpret_cast<GieNodeDataModel*>(c.getNode(QtNodes::PortType::Out)->nodeDataModel());
    auto receiver = reinterpret_cast<GieNodeDataModel*>(c.getNode(QtNodes::PortType::In)->nodeDataModel());

    auto portIndex = c.getPortIndex(QtNodes::PortType::Out);

    auto node = m_program.getNode(receiver->nodeId());
    node.m_logic.m_argument[portIndex] = giver->nodeId();

    m_program.editNode(receiver->nodeId(), node);

    //std::cout << "onConnectionCreated: connected " << giver->nodeId() << " with " << receiver->nodeId() << std::endl;
}

void Editor::onConnectionDeleted(const QtNodes::Connection& c)
{
    auto receiver = reinterpret_cast<GieNodeDataModel*>(c.getNode(QtNodes::PortType::In)->nodeDataModel());
    auto portIndex = c.getPortIndex(QtNodes::PortType::In);

    auto node = m_program.getNode(receiver->nodeId());
    node.m_logic.m_argument[portIndex] = NoArgument{};

    m_program.editNode(receiver->nodeId(), node);

    //std::cout << "onConnectionRemoved: removed argument from " << receiver->nodeId() << std::endl;
}