//
// Created by alex on 7/19/19.
//

#include "projectscripts.h"

#include <QVBoxLayout>
#include <QFileInfo>

#include <QDesktopServices>
#include <QUrl>

ProjectScripts::ProjectScripts(QWidget* parent):
    QWidget{parent}
{
    auto vlayout = new QVBoxLayout{this};
    vlayout->addWidget(m_openDirButton = new QPushButton{"Open scripts folder"});
    vlayout->addWidget(m_loadedFiles = new QListWidget{});

    m_openDirButton->setDisabled(true);
    connect(m_openDirButton, &QPushButton::clicked, this, &ProjectScripts::openInBrowser);
}

void ProjectScripts::addFile(const QString& name)
{
    if(m_files.count(name) == 0)
    {
        m_loadedFiles->addItem(name);
        m_files.insert(name);
    }
}

void ProjectScripts::removeFile(const QString& name)
{
    if(m_files.count(name))
    {
        for(int i = 0; i < m_loadedFiles->count(); i++)
        {
            if(m_loadedFiles->item(i)->text() == name)
            {
                delete m_loadedFiles->takeItem(i);
                break;
            }
        }

        m_files.erase(name);
    }
}

void ProjectScripts::removeAll()
{
    m_loadedFiles->clear();
}

void ProjectScripts::setScriptsFolder(QString path)
{
    m_scriptsDirectory = std::move(path);
    m_openDirButton->setDisabled(false);
}

void ProjectScripts::removeScriptsFolder()
{
    m_scriptsDirectory = std::nullopt;
    m_openDirButton->setDisabled(true);
}

void ProjectScripts::openInBrowser()
{
    if(m_scriptsDirectory)
        QDesktopServices::openUrl(QUrl(QString("file:///") + *m_scriptsDirectory));
}