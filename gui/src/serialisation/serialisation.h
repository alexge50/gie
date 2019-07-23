//
// Created by alex on 5/23/19.
//

#ifndef GUI_SERIALISATION_H
#define GUI_SERIALISATION_H

#include <nodes/FlowScene>

#include <QtCore/QJsonObject>

#include "src/Project.h"

auto serialise(const QtNodes::FlowScene& scene) -> QJsonObject;
std::map<QUuid, std::string> deserialise(QtNodes::FlowScene& scene, const Project& project, const QJsonObject& json);

#endif //GUI_SERIALISATION_H
