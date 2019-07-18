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
    void imagePressed(const ProjectImage&);

public Q_SLOTS:
    void onImagesUpdate(const std::vector<ProjectImage>&);

private Q_SLOTS:
    void cellTriggered(int row, int column);

private:
    QTableWidget* m_table;

    std::vector<ProjectImage> m_rows;
};


#endif //GUI_IMPORTEDIMAGESVIEWER_H
