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

    connect(m_table, &QTableWidget::cellDoubleClicked, this, &ImportedImagesViewer::cellTriggered);
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
        auto cell = new ImageCell(name, image, m_table);
        m_table->setCellWidget(row, 0, cell);
        m_rows.push_back(cell);
        row++;
    }

    m_table->resizeRowsToContents();
}

void ImportedImagesViewer::cellTriggered(int row, int column)
{
    Q_EMIT imagePressed(m_rows[row]->image());
}
