//
// Created by alex on 5/25/19.
//

#ifndef GUI_PROJECT_H
#define GUI_PROJECT_H

#include <QString>
#include <QDir>
#include <QUuid>
#include <QImage>
#include <QJsonObject>

#include <nodes/FlowScene>

#include <string>

struct ProjectImage
{
    QUuid id;
    QString name;
    QImage image;
};

class Editor;

class Project
{
private:
    Project(QtNodes::FlowScene& scene, QDir);
    Project(QtNodes::FlowScene& scene, QDir, QJsonObject);

public:
    Project(const Project&) = delete;
    Project(Project&&) = default;

    void save();
    void importImage(QString filename, QString name);

    const std::map<QUuid, ProjectImage>& importedImages() const { return m_images; };
    QString projectName() const { return m_name; }

    const QDir& projectPath() const;

private:
    QJsonValue serialiseImages();
    QJsonValue serialiseScripts();

    void deserialiseImages(QJsonValue);
    void deserialiseScripts(QJsonValue);

private:
    std::map<QUuid, ProjectImage> m_images;

    QtNodes::FlowScene& m_scene;
    QDir m_projectDirectory;
    QString m_name;

    friend Project newProject(QDir dir, QString name, QtNodes::FlowScene& scene);
    friend Project loadProject(QString directory, Editor&, QtNodes::FlowScene& scene);
};

Project newProject(QDir dir, QString name, QtNodes::FlowScene& scene);
Project loadProject(QString directory, Editor&, QtNodes::FlowScene& scene);

#endif //GUI_PROJECT_H
