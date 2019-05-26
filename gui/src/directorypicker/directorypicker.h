//
// Created by alex on 5/26/19.
//

#ifndef GUI_DIRECTORYPICKER_H
#define GUI_DIRECTORYPICKER_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

class DirectoryPicker: public QWidget
{
Q_OBJECT
public:
    explicit DirectoryPicker(QString title = "Select Directory", QWidget *parent = nullptr);

Q_SIGNALS:
    void directoryChanged(QString name);

private Q_SLOTS:
    void onSelectDirectory();
    void onDirectoryChanged();

private:
    bool isValidFile(const QString&);

private:
    QLineEdit* m_directoryEdit;
    QPushButton* m_selectDirectory;

    const QString m_title;
    QString m_directory;
};

#endif //GUI_DIRECTORYPICKER_H
