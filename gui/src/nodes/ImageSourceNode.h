//
// Created by alex on 7/17/19.
//

#ifndef GIE_IMAGESOURCENODE_H
#define GIE_IMAGESOURCENODE_H

#include "src/nodeeditor/SourceNode.h"
#include "src/widgets/filepicker/filepicker.h"

class ImageSourceNode: public SourceNode
{
    Q_OBJECT
public:
    ImageSourceNode():
        m_filePicker(new FilePicker("Images (*.png *.xpm *.jpg)"))
    {
        m_filePicker->setMaximumSize(m_filePicker->sizeHint());

        connect(
                m_filePicker, &FilePicker::fileChanged,
                this, &ImageSourceNode::onFileChanged
        );
    }

    ~ImageSourceNode() override = default;

public:
    QString caption() const override { return QStringLiteral("ImageSource"); }
    bool captionVisible() const override { return false; }
    QString name() const override { return QStringLiteral("ImageSource"); }

public:
    QJsonObject save() const override
    {
        QJsonObject modelJson = NodeDataModel::save();

        modelJson["file"] = m_filename;

        return modelJson;
    }

    void restore(QJsonObject const &p) override
    {
        QJsonValue filename = p["file"];

        if (!filename.isUndefined())
        {
            m_filePicker->setFile(filename.toString());
            onFileChanged(filename.toString());
        }
    }

public:
    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override
    {
        return ImageTypeData().type();
    }


    std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override
    {
        return std::make_shared<ImageTypeData>();
    }

    QWidget* embeddedWidget() override { return m_filePicker; }

    Data getData() override
    {
        QImage image = QImage(m_filename).convertToFormat(QImage::Format_RGB888);

        return Image{imageToRawData(image), static_cast<unsigned int>(image.width()),
                     static_cast<unsigned int>(image.height())};
    }

private Q_SLOTS:
    void onFileChanged(const QString& filename)
    {
        m_filename = filename;
        QImage image = QImage(m_filename).convertToFormat(QImage::Format_RGB888);


        Q_EMIT dataChanged(Data{Image{imageToRawData(image), static_cast<unsigned int>(image.width()),
                                       static_cast<unsigned int>(image.height())}});
        Q_EMIT dataUpdated(0);
    }

private:
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

private:
    FilePicker* m_filePicker;
    QString m_filename;
};

#endif //GIE_IMAGESOURCENODE_H
