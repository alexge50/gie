//
// Created by alex on 5/26/19.
//

#include "importedimagesviewer.h"

#include <QGridLayout>
#include <QHeaderView>

ImportedImagesViewer::ImportedImagesViewer(QWidget *parent):
    QWidget(parent),
    m_table{new QTableWidget}
{
    auto layout = new QVBoxLayout(this);
    layout->addWidget(m_table);
}

void ImportedImagesViewer::onImagesUpdate(const std::vector<ProjectImage>& images)
{
    m_table->reset();

    m_table->horizontalHeader()->setStretchLastSection(true);

    m_table->horizontalHeader()->hide();
    m_table->verticalHeader()->hide();

    m_table->setRowCount(static_cast<int>(images.size()));
    m_table->setColumnCount(1);

    int row = 0;
    for(const auto&[uuid, name, image]: images)
    {
        m_table->setCellWidget(row, 0, new ImageCell(name, image, m_table));
        row++;
    }

    m_table->resizeRowsToContents();
}

void ImportedImagesViewer::onImagePressed(const QImage& image)
{
    Q_EMIT imagePressed(image);
}
