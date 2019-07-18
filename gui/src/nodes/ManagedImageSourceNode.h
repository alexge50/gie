//
// Created by alex on 7/18/19.
//

#ifndef GIE_MANAGEDIMAGESOURCENODE_H
#define GIE_MANAGEDIMAGESOURCENODE_H

#include <QImage>
#include "src/nodeeditor/SourceNode.h"
#include "src/Project.h"

class ManagedImageSourceNode: public SourceNode
{
    Q_OBJECT
public:
    explicit ManagedImageSourceNode(const ProjectImage& projectImage):
            m_filename{projectImage.name},
            m_uuid{projectImage.id.toString()}
    {
        QImage image = projectImage.image.convertToFormat(QImage::Format_RGB888);
        auto gieImage = Image{imageToRawData(image), static_cast<unsigned int>(image.width()),
                              static_cast<unsigned int>(image.height())};

        m_image = std::move(gieImage);

        ok();
    }

    ~ManagedImageSourceNode() override = default;

public:
    QString caption() const override { return m_filename; }
    bool captionVisible() const override { return true; }
    QString name() const override { return QStringLiteral("ManagedImageSource"); }

public:
    QJsonObject save() const override
    {
        QJsonObject modelJson = NodeDataModel::save();
        modelJson["uuid"] = m_uuid;
        return modelJson;
    }

    void restore(QJsonObject const &p) override {}

public:
    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override
    {
        return ImageTypeData().type();
    }

    std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override
    {
        return std::make_shared<ImageTypeData>();
    }

    QWidget* embeddedWidget() override { return nullptr; }

    Data getData() override { return m_image; }

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
    QString m_filename;
    QString m_uuid;
    Image m_image;

    friend class Editor;
};

#endif //GIE_MANAGEDIMAGESOURCENODE_H
