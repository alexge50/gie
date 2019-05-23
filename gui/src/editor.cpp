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
#include <QtWidgets/QFileDialog>

#include "serialisation/serialise.h"
#include "serialisation/deserialise.h"

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
    /*if(auto* p = dynamic_cast<NumberSourceDataModel*>(n.nodeDataModel()); p != nullptr)
        QObject::connect(
            p, &NumberSourceDataModel::onValueChanged,
            this, &Editor::
        );*/
}

void Editor::onSave()
{
    auto filename = QFileDialog::getSaveFileName(
            this,
            tr("Save gie project file"),
            QDir::homePath(),
            tr("*.gie")
    );

    if(QFile file(filename); file.open(QIODevice::ReadWrite))
    {
        auto data = serialise(*m_scene);;
        file.write(data);
    }
}

void Editor::onLoad()
{
    auto filename = QFileDialog::getOpenFileName(
            this,
            tr("Open gie project file"),
            QDir::homePath(),
            tr("*.gie")
    );

    if(QFile file(filename); file.open(QIODevice::ReadOnly))
    {
        auto data = file.readAll();
        deserialise(*m_scene, data);
    }
}