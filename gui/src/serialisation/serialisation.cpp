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

void deserialise(QtNodes::FlowScene& scene, const QJsonObject& json)
{
    scene.clearScene();

    auto nodes = json["nodes"].toArray();
    for(QJsonValueRef node: nodes)
        scene.restoreNode(node.toObject());

    auto connections = json["connections"].toArray();
    for(QJsonValueRef connection: connections)
        scene.restoreConnection(connection.toObject());
}


#endif //GUI_SERIALISE_H
