//
// Created by alex on 5/24/19.
//

#include "colorpicker.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

ColorPicker::ColorPicker(QWidget *parent) : color_widgets::ColorWheel(parent)
{
    connect(this, &color_widgets::ColorWheel::colorSelected, this, &ColorPicker::onColorChanged);
}