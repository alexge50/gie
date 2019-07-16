//
// Created by alex on 5/25/19.
//

#include "ImageSourceDataModel.h"

ImageSourceDataModel::ImageSourceDataModel(): m_filePicker(new FilePicker("Images (*.png *.xpm *.jpg)"))
{
    m_filePicker->setMaximumSize(m_filePicker->sizeHint());

    connect(
            m_filePicker, &FilePicker::fileChanged,
            this, &ImageSourceDataModel::onFileChanged
    );
}

QJsonObject ImageSourceDataModel::save() const
{
    QJsonObject modelJson = NodeDataModel::save();

    modelJson["file"] = m_filename;

    return modelJson;
}

void ImageSourceDataModel::restore(QJsonObject const &p)
{
    QJsonValue filename = p["file"];

    if (!filename.isUndefined())
    {
        m_filePicker->setFile(filename.toString());
        onFileChanged(filename.toString());
    }
}

unsigned int ImageSourceDataModel::nPorts(QtNodes::PortType portType) const
{
    if(portType == QtNodes::PortType::In)
        return 0;
    else return 1;
}

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

void ImageSourceDataModel::onFileChanged(QString filename)
{
    m_filename = std::move(filename);
    QImage image = QImage(m_filename).convertToFormat(QImage::Format_RGB888);


    Q_EMIT valueChanged(Data{Image{imageToRawData(image), static_cast<unsigned int>(image.width()),
                                   static_cast<unsigned int>(image.height())}});
    Q_EMIT dataUpdated(0);
}

QtNodes::NodeDataType ImageSourceDataModel::dataType(QtNodes::PortType, QtNodes::PortIndex) const
{
    return ImageTypeData().type();
}


std::shared_ptr<QtNodes::NodeData> ImageSourceDataModel::outData(QtNodes::PortIndex)
{
    return std::make_shared<ImageTypeData>(m_valueId);
}