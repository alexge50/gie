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

#include "Project.h"
#include "Gie.h"

#include <src/nodeeditor/NodeEditor.h>

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

private:
    void keyPressEvent(QKeyEvent*) override;
    void reloadImages();

private:
    std::unique_ptr<Project> m_project;
    Gie* m_gie;
    NodeEditor* m_nodeEditor;

    QLabel* m_noProjectMessage;

    std::map<QUuid, QString> m_targets;
    std::map<QUuid, Data> m_values;
};


#endif //GUI_EDITOR_H
