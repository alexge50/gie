//
// Created by alex on 5/24/19.
//

#ifndef GUI_COLORSAMPLE_H
#define GUI_COLORSAMPLE_H

#include <QWidget>
#include <QPainter>
#include <QColor>

class ColorSample : public QWidget
{
    Q_OBJECT
public:
    explicit ColorSample(QWidget *parent = nullptr);

    void setColor(QColor);
    QColor color();

protected:
    void paintEvent(QPaintEvent*) override;

private:
    QColor m_color;
};

#endif //GUI_COLORSAMPLE_H
