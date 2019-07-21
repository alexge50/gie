//
// Created by alex on 5/25/19.
//

#ifndef GUI_IMAGEVIEWER_H
#define GUI_IMAGEVIEWER_H

#include <QWidget>

class ImageViewer : public QWidget
{
Q_OBJECT
public:
    explicit ImageViewer(QWidget *parent = nullptr);

    void setImage(QImage image);

protected:
    void paintEvent(QPaintEvent*) override;

private:
    QImage m_image;
};

#endif //GUI_IMAGEVIEWER_H
