//
// Created by alex on 7/17/19.
//

#ifndef GIE_IMAGEDISPLAYNODE_H
#define GIE_IMAGEDISPLAYNODE_H

#include "src/nodeeditor/DisplayNode.h"
#include "src/widgets/imageviewer/imageviewer.h"

class ImageDisplayNode: public DisplayNode
{
Q_OBJECT
public:
    ImageDisplayNode():
            m_imageViewer{new ImageViewer()}
    {
        m_imageViewer->setMaximumSize(70, 50);
    }

    virtual ~ImageDisplayNode() override = default;

public:
    QString caption() const override { return QString("ColorDisplay"); }
    bool captionVisible() const override { return false; }
    QString name() const override { return QString("ColorDisplay"); }

public:
    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override
    {
        return ImageTypeData().type();
    }

    QWidget* embeddedWidget() override { return m_imageViewer; }

public Q_SLOTS:
    void displayData(Data data) override
    {
        const auto& image = std::get<Image>(data);

		auto bufferSize = image.width() * image.height() * 3;
		auto imageData = new uint8_t[bufferSize];
		std::memcpy(imageData, image.raw(), bufferSize);

		QImage qImage(
		        imageData,
		        image.width(),
		        image.height(),
		        QImage::Format_RGB888,
		        [](auto p){ delete static_cast<uint8_t*>(p); }
		        );

		m_imageViewer->setDisplayType(image.width() > image.height() ? ImageViewer::FixedHeight : ImageViewer::FixedWidth);
		m_imageViewer->setImage(std::move(qImage));
    };

private:
    ImageViewer* m_imageViewer;
};

#endif //GIE_IMAGEDISPLAYNODE_H
