//
// Created by alex on 5/18/19.
//

#ifndef GUI_EDITOR_H
#define GUI_EDITOR_H

#include <memory>

#include <gie/Program.h>

#include <QWidget>
#include <QDockWidget>
#include <QLabel>

#include <nodes/FlowScene>
#include <nodes/FlowView>
#include "Project.h"

class Editor: public QWidget
{
    Q_OBJECT
public:
    explicit Editor(Program&, QWidget* parent = nullptr);

public:
    void setRegistry(std::shared_ptr<QtNodes::DataModelRegistry>);

Q_SIGNALS:
    void attachDockWindow(QDockWidget*);
    void importedImagesReload(const std::vector<ProjectImage>&);

public Q_SLOTS:
    void onNewProject();
    void onOpenProject();
    void onImportImage();

private Q_SLOTS:
    void onNewProject_(QDir, QString name);
    void onImportImage_(QString name);

    void onConnectionCreated(const QtNodes::Connection& c);
    void onConnectionDeleted(const QtNodes::Connection& c);
    void nodeCreated(QtNodes::Node &n);

private:
    void keyPressEvent(QKeyEvent*) override;
    void reloadImages();

private:
    Program& m_program;
    std::unique_ptr<Project> m_project;

    QtNodes::FlowScene* m_scene;
    QtNodes::FlowView* m_view;
    QLabel* m_noProjectMessage;
};


#endif //GUI_EDITOR_H
