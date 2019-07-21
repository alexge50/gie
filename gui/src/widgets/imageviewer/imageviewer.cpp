//
// Created by alex on 5/25/19.
//

#include "imageviewer.h"

#include <QPainter>

ImageViewer::ImageViewer(QWidget *parent) : QWidget(parent)
{
}

void ImageViewer::setImage(QImage image) { m_image = std::move(image); Q_EMIT update(); }


void ImageViewer::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int width = this->width(), height = this->height();

    if(width == 0 || height == 0 || m_image.width() == 0 || m_image.height() == 0)
        return;

    if(m_displayType == FixedWidth)
    {
        height = m_image.height() * width / m_image.width();
        this->resize(width, height);
    }
    else if(m_displayType == FixedHeight)
    {
        width = m_image.width() * height / m_image.height();
        this->resize(width, height);
    }
    else if(m_displayType == Full)
    {
        this->resize(m_image.width(), m_image.height());
    }

    painter.drawImage(QRect{0, 0, this->width(), this->height()}, m_image);
}