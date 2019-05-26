#include <utility>

//
// Created by alex on 5/25/19.
//

#include "Project.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QtGui/QImage>

#include "src/serialisation/serialisation.h"

Project::Project(QtNodes::FlowScene& scene,QDir projectDirectory):
    m_scene{scene},
    m_projectDirectory{projectDirectory},
    m_name{m_projectDirectory.dirName()}
{}

Project::Project(QtNodes::FlowScene& scene,QDir projectDirectory, QJsonObject json):
    m_scene{scene},
    m_projectDirectory{projectDirectory},
    m_name{m_projectDirectory.dirName()}
{
    deserialiseImages(json);
}

void Project::save()
{
    QJsonObject json;

    json["scene"] = serialise(m_scene);
    json["images"] = serialiseImages();
    json["scripts"] = serialiseScripts();

    if(QFile file(m_projectDirectory.absolutePath() + "/" + "gieprojectfile"); file.open(QIODevice::WriteOnly))
        file.write(QJsonDocument(json).toJson());
}

void Project::importImage(QString filename, QString name)
{
    if(QFile::exists(filename))
    {
        QUuid uuid = QUuid::createUuid();
        QString importedFilename = QString("images/") + uuid.toString();
        QString absoluteImportedFilename = m_projectDirectory.absolutePath() + "/" + importedFilename;
        QFile::copy(filename, absoluteImportedFilename);

        m_images[uuid] = {
                uuid,
                std::move(name),
                QImage(absoluteImportedFilename)
        };
    }
}

QJsonValue Project::serialiseImages()
{
    QJsonArray images;

    for(const auto& [uuid, imageMetadata]: m_images)
    {
        QJsonObject o;

        o["id"] = uuid.toString();
        o["name"] = imageMetadata.name;
        o["relpath"] = QString("images/") + uuid.toString();

        images.push_back(o);
    }

    return images;
}

QJsonValue Project::serialiseScripts()
{
    return QJsonArray{};
}

void Project::deserialiseImages(QJsonValue value)
{
    for(auto image: value.toArray())
    {
        auto imageObject = image.toObject();
        auto uuid = QUuid(imageObject["id"].toString());

        m_images[uuid] = {
                uuid,
                imageObject["name"].toString(),
                QImage(m_projectDirectory.absolutePath() + "/" + imageObject["relpath"].toString())
        };
    }
}

void Project::deserialiseScripts(QJsonValue)
{

}

Project newProject(QDir dir, QString name, QtNodes::FlowScene& scene)
{
    dir.mkdir(name);
    QDir projectDir = dir.absolutePath();
    projectDir.cd(name);

    projectDir.mkdir("scripts");
    projectDir.mkdir("images");

    QFile file(projectDir.absolutePath() + "/gieprojectfile");

    file.open(QIODevice::WriteOnly);

    file.write(R"({
    "scene": {
        "connections": [],
         "nodes": []
    }
    "images": [],
    "scripts": []
})");

    return Project{scene, projectDir};
}

Project loadProject(QString directory, QtNodes::FlowScene& scene)
{
    QFile file(directory + "/gieprojectfile");
    file.open(QIODevice::ReadOnly);

    auto data = file.readAll();

    QJsonObject json = QJsonDocument::fromJson(data).object();

    deserialise(scene, json["scene"].toObject());

    return Project{scene, directory, std::move(json)};
}