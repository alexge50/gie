//
// Created by alex on 5/26/19.
//

#ifndef GUI_IMAGECELL_H
#define GUI_IMAGECELL_H

#include <QWidget>
#include <QLabel>

#include "src/widgets/imageviewer/imageviewer.h"

class ImageCell: public QWidget
{
    Q_OBJECT
public:
    ImageCell(QString name, QImage image, QWidget* parent = nullptr);

    const QImage& image() { return m_image; }

protected:
    void resizeEvent(QResizeEvent*) override;

private:
    QImage m_image;
    QString m_name;

    ImageViewer* m_imageViewer;
    QLabel* m_label;
};


#endif //GUI_IMAGECELL_H
