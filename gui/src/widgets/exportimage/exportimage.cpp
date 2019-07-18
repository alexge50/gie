//
// Created by alex on 5/28/19.
//

#include "exportimage.h"
#include "ui_exportimage.h"

#include <QLabel>

ExportImage::ExportImage(const std::map<QUuid, QString>& targets, QWidget* parent):
    QDialog(parent),
    ui{new Ui::ExportImage()},
    m_saveFilePicker{new SaveFilePicker},
    m_targets{new QComboBox}
{
    ui->setupUi(this);

    ui->content->addWidget(new QLabel("File: "));
    ui->content->addWidget(m_saveFilePicker);

    ui->content->addWidget(new QLabel("Target: "));
    ui->content->addWidget(m_targets);

    int index = 0;
    for(const auto&[id, name]: targets)
        m_targets->insertItem(index++, name, id);

    connect(
            ui->buttonBox, &QDialogButtonBox::accepted,
            this, &ExportImage::onAccepted
    );
}

ExportImage::~ExportImage()
{
    delete ui;
}

void ExportImage::onAccepted()
{
    Q_EMIT exportImage(m_targets->currentData().toUuid(), m_saveFilePicker->filename());
}