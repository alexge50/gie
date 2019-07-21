//
// Created by alex on 5/23/19.
//

#ifndef GUI_SERIALISE_H
#define GUI_SERIALISE_H

#include "serialisation.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonValueRef>

#include <nodes/Node>
#include <src/nodes/ManagedImageSourceNode.h>

QJsonObject serialise(const QtNodes::FlowScene& scene)
{
    QJsonObject json;

    QJsonArray nodes;
    for([[maybe_unused]]const auto& [id, node]: scene.nodes())
        nodes.append(node->save());
    json["nodes"] = nodes;

    QJsonArray connections;
    for([[maybe_unused]]const auto& [id, connection]: scene.connections())
    {
        auto jsonConnection = connection->save();

        if(!jsonConnection.isEmpty())
            connections.append(jsonConnection);
    }

    json["connections"] = connections;


    return json;
}

std::map<QUuid, std::string> deserialise(QtNodes::FlowScene& scene, const Project& project, const QJsonObject& json)
{
    scene.clearScene();

    std::map<QUuid, QtNodes::Node*> managedNodes;
    std::map<QUuid, std::string> deleted;

    auto nodes = json["nodes"].toArray();
    for(QJsonValueRef node: nodes)
    {
        if(node.toObject()["model"].toObject()["name"].toString() != "ManagedImageSource")
        {
            if(scene.registry().registeredModelCreators().count(node.toObject()["model"].toObject()["name"].toString()))
            {
                scene.restoreNode(node.toObject());
            }
            else deleted.insert({QUuid{node.toObject()["id"].toString()}, node.toObject()["model"].toObject()["name"].toString().toStdString()});
        }
        else
        {
            QUuid id = node.toObject()["id"].toString();
            const ProjectImage& projectImage = project.importedImages().find(QUuid{node.toObject()["model"].toObject()["uuid"].toString()})->second;
            QPointF position = {
                    node.toObject()["position"].toObject()["x"].toDouble(),
                    node.toObject()["position"].toObject()["y"].toDouble()
            };

            auto& node = scene.createNode(std::make_unique<ManagedImageSourceNode>(projectImage));
            managedNodes[id] = &node;
            scene.setNodePosition(node, position);
        }
    }

    auto connections = json["connections"].toArray();
    for(QJsonValueRef connection: connections)
    {
        if(deleted.count(connection.toObject()["out_id"].toString()) ||
           deleted.count(connection.toObject()["in_id"].toString()) )
            continue;

        QUuid id = connection.toObject()["out_id"].toString();
        if(managedNodes.find(id) == managedNodes.end())
            scene.restoreConnection(connection.toObject());
        else
        {
            QUuid inId = connection.toObject()["in_id"].toString();
            int inIndex = connection.toObject()["in_index"].toInt();
            int outIndex = connection.toObject()["out_index"].toInt();

            scene.createConnection(*scene.nodes().find(inId)->second, inIndex, *managedNodes[id], outIndex);
        }
    }

    return deleted;
}

#endif //GUI_SERIALISE_H
