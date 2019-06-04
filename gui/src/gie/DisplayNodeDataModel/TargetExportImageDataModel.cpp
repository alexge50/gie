//
// Created by alex on 5/28/19.
//

#include "TargetExportImageDataModel.h"
#include "src/gie/types/ImageData.h"

TargetExportImageDataModel::TargetExportImageDataModel():
        m_targetNameEdit{new QLineEdit()},
        m_id{QUuid::createUuid()},
        m_dock{new QDockWidget("Image Preview")},
        m_imageViewer{new ImageViewer(m_dock)}
{
    m_targetNameEdit->setText("insert target name");
    connect(m_targetNameEdit, &QLineEdit::textEdited, this, &TargetExportImageDataModel::onTargetNameChanged);
    m_dock->setWidget(m_imageViewer);
    m_dock->setMinimumSize(100, 100);
}

QJsonObject TargetExportImageDataModel::save() const
{
    QJsonObject modelJson = NodeDataModel::save();

    if(m_targetName != "")
        modelJson["target_name"] = m_targetName;

    return modelJson;
}


void TargetExportImageDataModel::restore(const QJsonObject& p)
{
    QJsonValue v = p["target_name"];

    if (!v.isUndefined())
    {
        QString str = v.toString();

        m_targetName = str;
        m_targetNameEdit->setText(str);
        m_dock->setWindowTitle(str);
    }
}


unsigned int TargetExportImageDataModel::nPorts(QtNodes::PortType portType) const
{
    if(portType == QtNodes::PortType::In)
        return 1;
    return 0;
}

QtNodes::NodeDataType TargetExportImageDataModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    return ImageData().type();
}

void TargetExportImageDataModel::setInData(std::shared_ptr<QtNodes::NodeData> input, QtNodes::PortIndex)
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

void TargetExportImageDataModel::onTargetNameChanged(const QString& name)
{
    m_targetName = name;
    m_dock->setWindowTitle(name);
    Q_EMIT targetNameChanged(m_id, name);
}