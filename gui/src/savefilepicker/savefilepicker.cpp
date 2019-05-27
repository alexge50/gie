//
// Created by alex on 5/28/19.
//

#include "savefilepicker.h"

#include <QHBoxLayout>
#include <QFileDialog>

SaveFilePicker::SaveFilePicker(QString filter, QWidget *parent):
        QWidget(parent),
        m_filter{std::move(filter)}
{
    auto layout = new QHBoxLayout(this);

    layout->addWidget(m_filenameEdit = new QLineEdit(), 3);
    layout->addWidget(m_selectFile = new QPushButton("Open..."), 1);

    connect(
            m_filenameEdit, &QLineEdit::textEdited,
            this, &SaveFilePicker::filenameChanged
    );

    connect(
            m_selectFile, &QPushButton::pressed,
            this, &SaveFilePicker::selectFile
    );
}


void SaveFilePicker::selectFile()
{
    m_filename = QFileDialog::getSaveFileName(
            nullptr,
            tr("Save File"),
            QDir::homePath(),
            m_filter
    );

    m_filenameEdit->setText(m_filename);
    Q_EMIT fileChanged(m_filename);
}

void SaveFilePicker::filenameChanged()
{
    m_filename = m_filenameEdit->text();
    Q_EMIT fileChanged(m_filename);
}
