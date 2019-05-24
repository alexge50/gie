//
// Created by alex on 5/24/19.
//

#include "colorpicker.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

ColorPicker::ColorPicker(QWidget *parent) : QWidget(parent)
{
    auto colorEditor = new QGridLayout(this);

    colorEditor->setContentsMargins(QMargins());

    QString labels[] = {"R", "G", "B"};
    for(int row = 0; row < 3; row++)
    {
        QLabel* label = new QLabel(labels[row]);

        m_channelSelector[row] = new QSlider{Qt::Orientation::Horizontal};
        m_channelSelector[row]->setRange(0, 255);
        m_channelSelector[row]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        //m_channelSelector[row]->setArrowDirection(Qt::UpArrow);

        m_channelSpinbox[row] = new QSpinBox{};
        m_channelSpinbox[row]->setRange(0, 255);
        m_channelSpinbox[row]->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);


        connect(m_channelSelector[row], &KGradientSelector::valueChanged, this, &ColorPicker::onUpdate);
        connect(m_channelSelector[row], &KGradientSelector::valueChanged, m_channelSpinbox[row], &QSpinBox::setValue);
        connect(m_channelSpinbox[row], static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), m_channelSelector[row], &KGradientSelector::setValue);


        colorEditor->addWidget(label, row, 0);
        colorEditor->addWidget(m_channelSelector[row], row, 1);
        colorEditor->addWidget(m_channelSpinbox[row], row, 2);
    }

    onUpdate();
}

void ColorPicker::onUpdate()
{
    QRgb color = this->color().rgb();
    for (int row = 0; row < 3; row++)
    {
        int selectorMask = 0xFF << ((2 - row) * 8);
        auto minColor = QColor{color & (0xFFFFFF ^ selectorMask)};
        auto maxColor = QColor{color | selectorMask};
        //m_channelSelector[row]->setColors(minColor, maxColor);
    }

    Q_EMIT onColorChanged(this->color());
}

QColor ColorPicker::color()
{
    return {
            m_channelSpinbox[0]->value(),
            m_channelSpinbox[1]->value(),
            m_channelSpinbox[2]->value()
    };
}
