//
// Created by alex on 7/17/19.
//

#ifndef GIE_NODEEDITOR_H
#define GIE_NODEEDITOR_H

#include <QVBoxLayout>

#include <nodes/FlowScene>
#include <nodes/FlowView>
#include <nodes/NodeDataModel>
#include <nodes/Node>
#include <nodes/DataModelRegistry>
#include <nodes/ConnectionStyle>

#include "BaseNode.h"
#include "SourceNode.h"
#include "DisplayNode.h"
#include "ComputeNode.h"

class NodeEditor: public QWidget
{
    Q_OBJECT
public:
    explicit NodeEditor(QWidget* parent = nullptr):
        QWidget{parent},
        m_scene{new QtNodes::FlowScene},
        m_view{new QtNodes::FlowView{m_scene}},
        m_registry{std::make_shared<QtNodes::DataModelRegistry>()}
    {
        auto vlayout = new QVBoxLayout(this);

        vlayout->setMargin(0);
        vlayout->setSpacing(0);

        m_view->setSceneRect(-640000, -640000, 640000, 640000);
        m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        vlayout->addWidget(m_view);

        m_scene->setRegistry(m_registry);

        connect(
                m_scene, &QtNodes::FlowScene::connectionCreated,
                this, &NodeEditor::onConnectionCreated
        );

        connect(
                m_scene, &QtNodes::FlowScene::connectionDeleted,
                this, &NodeEditor::onConnectionDeleted
        );

        connect(
                m_scene, &QtNodes::FlowScene::nodeCreated,
                this, &NodeEditor::onNodeCreated
        );

        connect(
                m_scene, &QtNodes::FlowScene::nodeDeleted,
                this, &NodeEditor::onNodeDeleted
        );

        connect(m_scene, &QtNodes::FlowScene::nodePlaced, this, &NodeEditor::sceneChanged);
        connect(m_scene, &QtNodes::FlowScene::nodeDeleted, this, &NodeEditor::sceneChanged);
        connect(m_scene, &QtNodes::FlowScene::connectionCreated, this, &NodeEditor::sceneChanged);
        connect(m_scene, &QtNodes::FlowScene::connectionDeleted, this, &NodeEditor::sceneChanged);
        connect(m_scene, &QtNodes::FlowScene::nodeMoved, this, &NodeEditor::sceneChanged);
    }

    template <typename Creator>
    void registerNodeType(Creator&& creator, const QString& category)
    {
        using type = decltype(creator());
        m_registry->registerModel<type>(std::forward<Creator>(creator), category);
    }

    template <typename Type>
    void registerNodeType(const QString& category)
    {
        m_registry->registerModel<Type>(category);
    }

    QtNodes::FlowScene* scene() { return m_scene; }

Q_SIGNALS:
    void nodeCreated(BaseNode*);
    void nodeDeleted(BaseNode*);
    void argumentRemoved(BaseNode*, std::size_t port);
    void argumentEdited(BaseNode*, QUuid argumentId, std::size_t port);
    void sourceDataChanged(QUuid, Data);

    void sceneChanged();

public Q_SLOTS:
    void changeDisplayData(QUuid id, Data data)
    {
        dynamic_cast<BaseNode*>(m_scene->nodes().at(id)->nodeDataModel())->displayData(std::move(data));
    }

private Q_SLOTS:
    void onNodeCreated(QtNodes::Node* node)
    {
        if(auto* p = dynamic_cast<BaseNode*>(node->nodeDataModel()); p != nullptr)
        {
            p->m_id = node->id();

            connect(p, &BaseNode::dataChanged, [this, p](Data data)
            {
                Q_EMIT sourceDataChanged(p->id(), data);
            });

            Q_EMIT nodeCreated(p);
        }
    }

    void onNodeDeleted(QtNodes::Node* node)
    {
        if(auto* p = dynamic_cast<BaseNode*>(node->nodeDataModel()); p != nullptr)
        {
            Q_EMIT nodeDeleted(p);
        }
    }

    void onConnectionCreated(const QtNodes::Connection& c)
    {
        auto giver = dynamic_cast<BaseNode*>(c.getNode(QtNodes::PortType::Out)->nodeDataModel());
        auto receiver = dynamic_cast<BaseNode*>(c.getNode(QtNodes::PortType::In)->nodeDataModel());

        if(giver == nullptr || receiver == nullptr)
            return;

        Q_EMIT argumentEdited(
                receiver,
                giver->id(),
                static_cast<std::size_t>(c.getPortIndex(QtNodes::PortType::In))
                );
    }

    void onConnectionDeleted(const QtNodes::Connection& c)
    {
        auto giver = dynamic_cast<BaseNode*>(c.getNode(QtNodes::PortType::Out)->nodeDataModel());
        auto receiver = dynamic_cast<BaseNode*>(c.getNode(QtNodes::PortType::In)->nodeDataModel());

        if(giver == nullptr || receiver == nullptr)
            return;

        Q_EMIT argumentRemoved(
                receiver,
                static_cast<std::size_t>(c.getPortIndex(QtNodes::PortType::In))
        );
    }

private:
    QtNodes::FlowScene* m_scene;
    QtNodes::FlowView* m_view;

    std::shared_ptr<QtNodes::DataModelRegistry> m_registry;
};


#endif //GIE_NODEEDITOR_H
