//
// Created by alex on 5/26/19.
//

#ifndef GUI_NEWPROJECT_H
#define GUI_NEWPROJECT_H

#include <QDialog>
#include <QLineEdit>

#include <QDir>
#include <QString>
#include "src/widgets/directorypicker/directorypicker.h"

namespace Ui
{
    class NewProject;
}

class NewProject: public QDialog
{
    Q_OBJECT
public:
    NewProject(QWidget* parent = nullptr);
    ~NewProject();

Q_SIGNALS:
    void newProject(QDir, QString name);

private Q_SLOTS:
    void onDirectoryChanged(const QString&);
    void onNameChanged(const QString&);
    void onAccepted();

private:
    Ui::NewProject* ui;
    QLineEdit* m_nameEdit;
    DirectoryPicker* m_baseDirectory;

    QString m_directory, m_name;
};


#endif //GUI_NEWPROJECT_H
