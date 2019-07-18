//
// Created by alex on 5/25/19.
//

#ifndef GUI_FILEPICKER_H
#define GUI_FILEPICKER_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

class FilePicker : public QWidget
{
Q_OBJECT
public:
    explicit FilePicker(QString filter = "*", QWidget *parent = nullptr);

    void setFile(QString name);

Q_SIGNALS:
    void fileChanged(QString name);

private Q_SLOTS:
    void selectFile();
    void filenameChanged();

private:
    bool isValidFile(const QString&);

private:
    QLineEdit* m_filenameEdit;
    QPushButton* m_selectFile;

    const QString m_filter;
    QString m_filename;
};

#endif //GUI_FILEPICKER_H
