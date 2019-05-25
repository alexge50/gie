//
// Created by alex on 5/23/19.
//

#ifndef GUI_DESERIALISE_H
#define GUI_DESERIALISE_H

#include <nodes/FlowScene>
#include <QByteArray>

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonValueRef>

#include <unordered_set>
#include <string>

void deserialise(QtNodes::FlowScene& scene, const QByteArray& data)
{
    scene.clearScene();

    QJsonObject json = QJsonDocument::fromJson(data).object();

    auto nodes = json["nodes"].toArray();
    for(QJsonValueRef node: nodes)
        scene.restoreNode(node.toObject());

    auto connections = json["connections"].toArray();
    for(QJsonValueRef connection: connections)
        scene.restoreConnection(connection.toObject());
}

#endif //GUI_DESERIALISE_H
