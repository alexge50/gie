//
// Created by alex on 5/26/19.
//

#ifndef GUI_IMPORTIMAGE_H
#define GUI_IMPORTIMAGE_H

#include <QDialog>
#include <QLineEdit>

#include <QDir>
#include <QString>
#include "src/filepicker/filepicker.h"
#include "src/imageviewer/imageviewer.h"

namespace Ui
{
    class ImportImage;
}

class ImportImage: public QDialog
{
    Q_OBJECT
public:
    ImportImage(QWidget* parent = nullptr);
    ~ImportImage();

Q_SIGNALS:
    void importImage(QString name);

private Q_SLOTS:
    void onNameChanged(const QString&);
    void onAccepted();

private:
    Ui::ImportImage* ui;
    FilePicker* m_imagePicker;
    ImageViewer* m_preview;

    QString m_filename;
};

#endif //GUI_IMPORTIMAGE_H
