//
// Created by alex on 5/26/19.
//

#ifndef GUI_IMPORTEDIMAGESVIEWER_H
#define GUI_IMPORTEDIMAGESVIEWER_H

#include <QWidget>
#include <QTableWidget>

#include "src/Project.h"
#include "imagecell.h"

class ImportedImagesViewer: public QWidget
{
    Q_OBJECT

public:
    ImportedImagesViewer(QWidget* parent = nullptr);

Q_SIGNALS:
    void imagePressed(const QImage&);

public Q_SLOTS:
    void onImagesUpdate(const std::vector<ProjectImage>&);

private Q_SLOTS:
    void onImagePressed(const QImage&);

private:
    QTableWidget* m_table;
};


#endif //GUI_IMPORTEDIMAGESVIEWER_H
