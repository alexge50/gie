//
// Created by alex on 7/19/19.
//

#ifndef GIE_PROJECTSCRIPTS_H
#define GIE_PROJECTSCRIPTS_H

#include <optional>

#include <QWidget>
#include <QPushButton>
#include <QListWidget>

class ProjectScripts: public QWidget
{
    Q_OBJECT
public:
    explicit ProjectScripts(QWidget* parent = nullptr);

public Q_SLOTS:
    void addFile(const QString& name);
    void removeFile(const QString& name);
    void removeAll();

    void setScriptsFolder(QString path);
    void removeScriptsFolder();

private:
    void openInBrowser();

private:
    QListWidget* m_loadedFiles;
    QPushButton* m_openDirButton;
    std::optional<QString> m_scriptsDirectory;
};


#endif //GIE_PROJECTSCRIPTS_H
