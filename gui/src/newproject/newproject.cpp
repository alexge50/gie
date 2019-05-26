//
// Created by alex on 5/26/19.
//

#include "newproject.h"
#include "ui_newproject.h"

#include <QLabel>

NewProject::NewProject(QWidget* parent):
    QDialog(parent),
    ui{new Ui::NewProject()}
{
    this->setWindowTitle("New Project");

    ui->setupUi(this);

    ui->content->addWidget(new QLabel("Directory"));
    ui->content->addWidget(m_baseDirectory = new DirectoryPicker());

    ui->content->addWidget(new QLabel("Name"));
    ui->content->addWidget(m_nameEdit = new QLineEdit("Untitled"));

    connect(
            m_baseDirectory, &DirectoryPicker::directoryChanged,
            this, &NewProject::onDirectoryChanged
    );

    connect(
            m_nameEdit, &QLineEdit::textEdited,
            this, &NewProject::onNameChanged
    );

    connect(
            ui->buttonBox, &QDialogButtonBox::accepted,
            this, &NewProject::onAccepted
    );
}

NewProject::~NewProject()
{
    delete ui;
}

void NewProject::onDirectoryChanged(const QString& directory)
{
    m_directory = directory;
}

void NewProject::onNameChanged(const QString& name)
{
    m_name = name;
}

#include <QDebug>

void NewProject::onAccepted()
{
    qDebug() << m_directory;
    qDebug() << m_name;

    Q_EMIT newProject(m_directory, m_name);
}
