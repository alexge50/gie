//
// Created by alex on 5/26/19.
//

#include "directorypicker.h"

#include <QHBoxLayout>
#include <QFileDialog>

DirectoryPicker::DirectoryPicker(QString title, QWidget *parent):
        QWidget(parent),
        m_title{std::move(title)}
{
    auto layout = new QHBoxLayout(this);

    layout->addWidget(m_directoryEdit = new QLineEdit(), 3);
    layout->addWidget(m_selectDirectory = new QPushButton("Open..."), 1);

    connect(
            m_directoryEdit, &QLineEdit::textEdited,
            this, &DirectoryPicker::onDirectoryChanged
    );

    connect(
            m_selectDirectory, &QPushButton::pressed,
            this, &DirectoryPicker::onSelectDirectory
    );
}

void DirectoryPicker::onSelectDirectory()
{
    m_directory = QFileDialog::getExistingDirectory(
            nullptr,
            m_title,
            QDir::homePath()
    );

    m_directoryEdit->setText(m_directory);
    Q_EMIT directoryChanged(m_directory);
}

void DirectoryPicker::onDirectoryChanged()
{
    if(isValidFile(m_directoryEdit->text()))
    {
        m_directory = m_directoryEdit->text();
        Q_EMIT directoryChanged(m_directory);
    }
    else m_directoryEdit->setText(m_directory);
}

bool DirectoryPicker::isValidFile(const QString&)
{
    return true;
}