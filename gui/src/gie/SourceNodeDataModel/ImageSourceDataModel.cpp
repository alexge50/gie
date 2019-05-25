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

    if (m_data)
    {
        modelJson["file"] = m_filename;
    }

    return modelJson;
}

void ImageSourceDataModel::restore(QJsonObject const &p)
{
    QJsonValue filename = p["file"];

    if (!filename.isUndefined())
    {
        onFileChanged(filename.toString());
    }
}

unsigned int ImageSourceDataModel::nPorts(QtNodes::PortType portType) const
{
    if(portType == QtNodes::PortType::In)
        return 0;
    else return 1;
}

void ImageSourceDataModel::onFileChanged(QString filename)
{
    m_filename = std::move(filename);
    QImage image(m_filename);

    m_data = std::make_shared<ImageData>(Image{image.bits(), static_cast<unsigned int>(image.width()),
                                               static_cast<unsigned int>(image.height())});
    Q_EMIT dataUpdated(0);
    Q_EMIT onValueChanged(m_data);
}

QtNodes::NodeDataType ImageSourceDataModel::dataType(QtNodes::PortType, QtNodes::PortIndex) const
{
    return ImageData().type();
}


std::shared_ptr<QtNodes::NodeData> ImageSourceDataModel::outData(QtNodes::PortIndex)
{
    return m_data;
}