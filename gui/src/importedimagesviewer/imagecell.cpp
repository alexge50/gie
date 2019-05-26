//
// Created by alex on 5/26/19.
//

#include "imagecell.h"

#include <QVBoxLayout>

ImageCell::ImageCell(QString name, QImage image, QWidget *parent):
    QWidget(parent),
    m_image{std::move(image)},
    m_name{std::move(name)}
{
    this->setMinimumSize(200, 200);
    auto layout = new QVBoxLayout(this);

    layout->addWidget(m_label = new QLabel(m_name), 1);
    layout->addWidget(m_imageViewer = new ImageViewer, 5);

    m_imageViewer->setDisplayType(m_image.width() > m_image.height() ? ImageViewer::FixedWidth : ImageViewer::FixedHeight);
    m_imageViewer->setImage(m_image);

    layout->setMargin(10);
}

void ImageCell::resizeEvent(QResizeEvent*)
{
    m_imageViewer->setDisplayType(width() > height() ? ImageViewer::FixedWidth : ImageViewer::FixedHeight);
}