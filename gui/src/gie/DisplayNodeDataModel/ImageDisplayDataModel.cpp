//
// Created by alex on 5/25/19.
//

#include "ImageDisplayDataModel.h"
#include "src/gie/types/ImageData.h"

ImageDisplayDataModel::ImageDisplayDataModel():
        m_imageViewer{new ImageViewer()}
{
    m_imageViewer->setMaximumSize(70, 50);
}

unsigned int ImageDisplayDataModel::nPorts(QtNodes::PortType portType) const
{
    if(portType == QtNodes::PortType::In)
        return 1;
    return 0;
}

QtNodes::NodeDataType ImageDisplayDataModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    return ImageData().type();
}

void ImageDisplayDataModel::setInData(std::shared_ptr<QtNodes::NodeData> input, QtNodes::PortIndex portIndex)
{
    if(auto* data = dynamic_cast<ImageData*>(input.get()); data)
    {
        modelValidationState = QtNodes::NodeValidationState::Valid;
        modelValidationError = QString();

        const auto& image = data->image();
        auto imageData = new uint8_t[image.width * image.height * 3];
        std::memcpy(imageData, image.raw(), image.width * image.height * 3);

        QImage qImage(imageData, image.width, image.height, QImage::Format_RGB888, [](auto p){ delete static_cast<uint8_t*>(p); });

        m_imageViewer->setDisplayType(image.width > image.height ? ImageViewer::FixedHeight : ImageViewer::FixedWidth);
        m_imageViewer->setImage(std::move(qImage));
    }
    else
    {
        modelValidationState = QtNodes::NodeValidationState::Valid;
        modelValidationError = QStringLiteral("Missing or incorrect inputs");

        m_imageViewer->setImage(QImage{});
    }
}
