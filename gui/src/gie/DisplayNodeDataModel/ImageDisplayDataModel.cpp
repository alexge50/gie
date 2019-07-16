//
// Created by alex on 5/25/19.
//

#include "ImageDisplayDataModel.h"
#include "src/gie/types/TypeData.h"

ImageDisplayDataModel::ImageDisplayDataModel():
        m_imageViewer{new ImageViewer()}
{
    m_imageViewer->setMaximumSize(70, 50);
}

QtNodes::NodeDataType ImageDisplayDataModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    return ImageTypeData().type();
}

void ImageDisplayDataModel::displayData(Data data)
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
}
