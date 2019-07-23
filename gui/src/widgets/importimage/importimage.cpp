//
// Created by alex on 5/26/19.
//

#include "importimage.h"
#include "ui_importimage.h"

#include <QLabel>

ImportImage::ImportImage(QWidget* parent):
        QDialog(parent),
        ui{new Ui::ImportImage()}
{
    this->setWindowTitle("New Project");

    ui->setupUi(this);

    ui->selectFileContent->addWidget(new QLabel("File"));
    ui->selectFileContent->addWidget(m_imagePicker = new FilePicker("Image file (*.png *.jpeg *.jpg *.xpm *.bmp)"));

    ui->previewContent->addWidget(m_preview = new ImageViewer());

    connect(
            m_imagePicker, &FilePicker::fileChanged,
            this, &ImportImage::onNameChanged
    );

    connect(
            ui->buttonBox, &QDialogButtonBox::accepted,
            this, &ImportImage::onAccepted
    );
}

ImportImage::~ImportImage()
{
    delete ui;
}

void ImportImage::onNameChanged(const QString& name)
{
    m_filename = name;

    QImage image(m_filename);
    m_preview->setImage(std::move(image));
}

#include <QDebug>

void ImportImage::onAccepted()
{
    qDebug() << m_filename;

    Q_EMIT importImage(m_filename);
}
