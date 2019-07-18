//
// Created by alex on 5/24/19.
//

#include "colorsample.h"

ColorSample::ColorSample(QWidget *parent) : QWidget(parent)
{

}

void ColorSample::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(m_color);
    painter.drawRect(0, 0, width(), height());
}

void ColorSample::setColor(QColor color)
{
    m_color = color;
}

QColor ColorSample::color()
{
    return m_color;
}
