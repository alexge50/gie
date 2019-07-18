//
// Created by alex on 5/24/19.
//

#ifndef GUI_COLORPICKER_H
#define GUI_COLORPICKER_H


#include <QWidget>
#include <QtColorWidgets/ColorWheel>

class ColorPicker: public color_widgets::ColorWheel
{
Q_OBJECT
public:
    explicit ColorPicker(QWidget *parent = nullptr);


Q_SIGNALS:
    void onColorChanged(QColor color);
};

#endif //GUI_COLORPICKER_H
