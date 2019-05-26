//
// Created by alex on 5/25/19.
//

#include "filepicker.h"

#include <QHBoxLayout>
#include <QFileDialog>

FilePicker::FilePicker(QString filter, QWidget *parent):
        QWidget(parent),
        m_filter{std::move(filter)}
{
    auto layout = new QHBoxLayout(this);

    layout->addWidget(m_filenameEdit = new QLineEdit(), 3);
    layout->addWidget(m_selectFile = new QPushButton("Open..."), 1);

    connect(
            m_filenameEdit, &QLineEdit::textEdited,
            this, &FilePicker::filenameChanged
    );

    connect(
            m_selectFile, &QPushButton::pressed,
            this, &FilePicker::selectFile
    );
}

void FilePicker::setFile(QString name)
{
    m_filenameEdit->setText(name);
}

void FilePicker::selectFile()
{
    m_filename = QFileDialog::getOpenFileName(
            nullptr,
            tr("Open File"),
            QDir::homePath(),
            m_filter
    );

    m_filenameEdit->setText(m_filename);
    Q_EMIT fileChanged(m_filename);
}

void FilePicker::filenameChanged()
{
    if(isValidFile(m_filenameEdit->text()))
    {
        m_filename = m_filenameEdit->text();
        Q_EMIT fileChanged(m_filename);
    }
    else m_filenameEdit->setText(m_filename);
}

bool FilePicker::isValidFile(const QString&)
{
    return true;
}
