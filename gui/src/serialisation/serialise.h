//
// Created by alex on 5/23/19.
//

#ifndef GUI_SERIALISE_H
#define GUI_SERIALISE_H

#include "../gie/GieNodeDataModel.h"
#include <nodes/FlowScene>

#include <QtCore/QJsonArray>
#include <QtCore/QJsonValue>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>

#include <iostream>

auto serialise(const QtNodes::FlowScene& scene)
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


    return QJsonDocument{json}.toJson();
}

#endif //GUI_SERIALISE_H
