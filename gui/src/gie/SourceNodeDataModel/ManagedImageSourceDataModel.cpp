//
// Created by alex on 5/26/19.
//

#include "ManagedImageSourceDataModel.h"

static std::vector<uint8_t> imageToRawData(const QImage& image)
{
    std::vector<uint8_t> data(static_cast<unsigned long>(image.width() * image.height() * 3));
    const auto rowSize = static_cast<const size_t>(image.width()) * 3;
    std::size_t index = 0;
    std::size_t indexInImage = 0;

    for(int i = 0; i < image.height(); i++)
    {
        std::memcpy(data.data() + index, image.bits() + indexInImage, rowSize);

        indexInImage += image.bytesPerLine();
        index += rowSize;
    }

    return data;
}

ManagedImageSourceDataModel::ManagedImageSourceDataModel(const ProjectImage& projectImage):
    m_filename{projectImage.name},
    m_uuid{projectImage.id.toString()}
{
    QImage image = projectImage.image.convertToFormat(QImage::Format_RGB888);

    m_data = std::make_shared<ImageData>(Image{imageToRawData(image), static_cast<unsigned int>(image.width()),
                                               static_cast<unsigned int>(image.height())});
}

QJsonObject ManagedImageSourceDataModel::save() const
{
    QJsonObject modelJson = NodeDataModel::save();

    if (m_data)
    {
        modelJson["uuid"] = m_uuid;
    }

    return modelJson;
}

void ManagedImageSourceDataModel::restore(QJsonObject const &p)
{}

unsigned int ManagedImageSourceDataModel::nPorts(QtNodes::PortType portType) const
{
    if(portType == QtNodes::PortType::In)
        return 0;
    else return 1;
}

QtNodes::NodeDataType ManagedImageSourceDataModel::dataType(QtNodes::PortType, QtNodes::PortIndex) const
{
    return ImageData().type();
}


std::shared_ptr<QtNodes::NodeData> ManagedImageSourceDataModel::outData(QtNodes::PortIndex)
{
    return m_data;
}