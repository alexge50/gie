//
// Created by alex on 5/18/19.
//

#include "editor.h"
#include <QVBoxLayout>

#include "gie/GieDataModelRegistry.h"

Editor::Editor(Program& program, QWidget* parent): QWidget(parent), m_program{program}
{
    auto vlayout = new QVBoxLayout(this);

    vlayout->setMargin(0);
    vlayout->setSpacing(0);

    m_scene = new QtNodes::FlowScene(registerDataModels(m_program));

    m_view = new QtNodes::FlowView(m_scene);
    m_view->setSceneRect(-640000, -640000, 640000, 640000);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    vlayout->addWidget(m_view);
}