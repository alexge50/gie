//
// Created by alex on 5/18/19.
//

#ifndef GUI_EDITOR_H
#define GUI_EDITOR_H

#include <memory>
#include <map>
#include <set>

#include <gie/Program.h>

#include <QWidget>
#include <QDockWidget>
#include <QLabel>
#include <QUuid>
#include <QFileSystemWatcher>

#include "Project.h"
#include "Gie.h"

#include <src/nodeeditor/NodeEditor.h>
#include "src/nodes/GieNode.h"

class Editor: public QWidget
{
    Q_OBJECT
public:
    explicit Editor(QWidget* parent = nullptr);

public:
    QString getProjectName();

    void loadModule(const QString&, const QString&);

    NodeEditor* nodeEditor() { return m_nodeEditor; }

Q_SIGNALS:
    void attachDockWindow(QDockWidget*);
    void detachDockWindow(QDockWidget*);
    void importedImagesReload(const std::vector<ProjectImage>&);
    void sceneChanged();
    void savedProject();

    void reloadedSymbols(std::vector<GieSymbol>);
    void projectLoaded(const Project&);

    void scriptAdded(QString name);
    void scriptRemoved(QString name);

public Q_SLOTS:
    void onNewProject();
    void onOpenProject();
    void onImportImage();

    void addImageNode(const ProjectImage&);

    void onExportImage();

private Q_SLOTS:
    void onNewProject_(QDir, QString name);
    void onImportImage_(QString name);
    void onExportImage_(const QUuid&, const QString&);

    void onTargetNameChanged(const QUuid& id, const QString&);

    void resultUpdated(QUuid, Data);
    void reloadNode(const std::vector<GieSymbol>& symbols);

    void nodeCreated(BaseNode*);
    void nodeDeleted(BaseNode*);
    void argumentRemoved(BaseNode* node, std::size_t port);
    void argumentEdited(BaseNode* node, QUuid argumentId, std::size_t port);
    void sourceDataChanged(QUuid nodeId, Data data);

    void runtimeError(const GieRuntimeError&);

private:
    void keyPressEvent(QKeyEvent*) override;
    void reloadImages();

private:
    void addScript(const QString& path);
    void removeScript(const QString& path);

private:
    std::unique_ptr<Project> m_project;
    Gie* m_gie;
    NodeEditor* m_nodeEditor;

    QFileSystemWatcher* m_projectScriptsWatcher;

    QLabel* m_noProjectMessage;

    std::map<QUuid, QString> m_targets;
    std::map<QUuid, Data> m_values;

    struct ToUpdate
    {
        QUuid id;
        std::size_t port{};

        bool operator< (const ToUpdate& other) const { return id < other.id; }
        bool operator== (const ToUpdate& other) const { return id == other.id; }
    };
    std::map<QUuid, std::set<ToUpdate>> m_toUpdate;

    std::map<QUuid, std::set<QUuid>> m_displaysToUpdate;

    std::map<QString, QDateTime> m_scripts;

    std::map<std::string, std::set<QUuid>> m_nodes;

    friend class Project loadProject(QString directory, Editor& editor, QtNodes::FlowScene& scene);
};


#endif //GUI_EDITOR_H
