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
        m_image = QImageToImage(projectImage.image);

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
    static Image QImageToImage(QImage image)
    {
        image = image.convertToFormat(QImage::Format_BGR888);

        Image result{static_cast<unsigned int>(image.width()), static_cast<unsigned int>(image.height())};

        for(int i = 0; i < image.height(); i++)
        {
            for(int j = 0; j < image.width(); j++)
            {
                auto color = QColor{image.pixel(j, i)};

                result.setPixel(i, j, {
                    static_cast<uint8_t>(color.red()),
                    static_cast<uint8_t>(color.green()),
                    static_cast<uint8_t>(color.blue())
                });
            }
        };

        [[maybe_unused]] auto ptr = result.raw();

        return result;
    }

private:
    QString m_filename;
    QString m_uuid;
    Image m_image;

    friend class Editor;
};

#endif //GIE_MANAGEDIMAGESOURCENODE_H
