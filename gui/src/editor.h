//
// Created by alex on 5/18/19.
//

#ifndef GUI_EDITOR_H
#define GUI_EDITOR_H

#include <gie/Program.h>
#undef B0

#include <QWidget>
#include <nodes/FlowScene>
#include <nodes/FlowView>

class Editor: public QWidget
{
    Q_OBJECT
public:
    explicit Editor(Program&, QWidget* parent = nullptr);

private:
    Program& m_program;
    QtNodes::FlowScene* m_scene;
    QtNodes::FlowView* m_view;
};


#endif //GUI_EDITOR_H
