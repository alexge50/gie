//
// Created by alex on 5/24/19.
//

#ifndef GUI_COLORPICKER_H
#define GUI_COLORPICKER_H


#include <QWidget>
#include <QSpinBox>
#include <QSlider>

class ColorPicker : public QWidget
{
Q_OBJECT
public:
    explicit ColorPicker(QWidget *parent = nullptr);

    QColor color();

Q_SIGNALS:
    void onColorChanged(const QColor& color);

private Q_SLOTS:
    void onUpdate();

private:
    //KGradientSelector* m_channelSelector[3];
    QSlider* m_channelSelector[3];
    QSpinBox* m_channelSpinbox[3];
};

#endif //GUI_COLORPICKER_H
