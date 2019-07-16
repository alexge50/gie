//
// Created by alex on 5/18/19.
//

#ifndef GUI_EDITOR_H
#define GUI_EDITOR_H

#include <memory>
#include <map>

#include <gie/Program.h>

#include <QWidget>
#include <QDockWidget>
#include <QLabel>
#include <QUuid>

#include <nodes/FlowScene>
#include <nodes/FlowView>
#include <src/gie/types/TypeData.h>
#include "Project.h"
#include "Gie.h"

class Editor: public QWidget
{
    Q_OBJECT
public:
    explicit Editor(QWidget* parent = nullptr);

public:
    void setRegistry(std::shared_ptr<QtNodes::DataModelRegistry>);

    QString getProjectName();

    void loadModule(const QString&, const QString&);

Q_SIGNALS:
    void attachDockWindow(QDockWidget*);
    void detachDockWindow(QDockWidget*);
    void importedImagesReload(const std::vector<ProjectImage>&);
    void sceneChanged();
    void savedProject();

    void reloadedSymbols(std::vector<GieSymbol>);

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

    void onConnectionCreated(const QtNodes::Connection& c);
    void onConnectionDeleted(const QtNodes::Connection& c);
    void nodeCreated(QtNodes::Node &node);
    void nodeDeleted(QtNodes::Node &node);

    void onTargetNameChanged(const QUuid& id, const QString&);

    void resultUpdated(QUuid, Data);

private:
    void keyPressEvent(QKeyEvent*) override;
    void reloadImages();

private:
    std::unique_ptr<Project> m_project;
    Gie* m_gie;

    QtNodes::FlowScene* m_scene;
    QtNodes::FlowView* m_view;
    QLabel* m_noProjectMessage;

    std::map<QUuid, QString> m_targets;
    std::map<QUuid, Data> m_values;
};


#endif //GUI_EDITOR_H
