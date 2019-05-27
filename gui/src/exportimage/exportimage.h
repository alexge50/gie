//
// Created by alex on 5/28/19.
//

#ifndef GUI_EXPORTIMAGE_H
#define GUI_EXPORTIMAGE_H

#include <QDialog>
#include <QComboBox>
#include <QUuid>

#include <map>

#include "src/savefilepicker/savefilepicker.h"

namespace Ui
{
    class ExportImage;
}

class ExportImage: public QDialog
{
    Q_OBJECT
public:
    ExportImage(const std::map<QUuid, QString>&, QWidget* parent = nullptr);
    ~ExportImage();

Q_SIGNALS:
    void exportImage(const QUuid& targetId, const QString& filename);

private Q_SLOTS:
    void onAccepted();

private:
    Ui::ExportImage* ui;

    SaveFilePicker* m_saveFilePicker;
    QComboBox* m_targets;
};


#endif //GUI_EXPORTIMAGE_H
