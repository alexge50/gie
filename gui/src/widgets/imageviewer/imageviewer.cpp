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

    double screenRatio = static_cast<double>(this->width()) / this->height();
    double imageRatio = static_cast<double>(m_image.width()) / m_image.height();

    QImage toPrint = imageRatio < screenRatio ? m_image.scaledToHeight(this->height()) : m_image.scaledToWidth(this->width());

    const QSize size = toPrint.size();
    const int x = (this->rect().width() - size.width()) / 2;
    const int y = (this->rect().height() - size.height()) / 2;
    painter.drawImage(QRect{x, y, size.width(), size.height()}, toPrint);
}