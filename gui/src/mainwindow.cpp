//
// Created by alex on 5/18/19.
//

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "editor.h"

/*
MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}*/

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(nullptr)
{
    m_program.context().module("builtins", false);

    auto sys = m_program.context().module("sys", false);
    auto os = m_program.context().module("os", false);

    sys.attr("path").attr("insert")(1, os.attr("getcwd")());

    m_program.import("modules.arithmetic");

    setCentralWidget(new Editor(m_program, this));
}

MainWindow::~MainWindow()
{
    delete ui;
}
