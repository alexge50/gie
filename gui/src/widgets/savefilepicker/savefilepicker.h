//
// Created by alex on 5/28/19.
//

#ifndef GUI_SAVEFILEPICKER_H
#define GUI_SAVEFILEPICKER_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

class SaveFilePicker : public QWidget
{
    Q_OBJECT
public:
    explicit SaveFilePicker(QString filter = "*", QWidget *parent = nullptr);

    QString filename() const { return m_filename; }

Q_SIGNALS:
    void fileChanged(QString name);

private Q_SLOTS:
    void selectFile();
    void filenameChanged();

private:
    QLineEdit* m_filenameEdit;
    QPushButton* m_selectFile;

    const QString m_filter;
    QString m_filename;
};

#endif //GUI_SAVEFILEPICKER_H
