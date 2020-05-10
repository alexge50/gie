//
// Created by alex on 7/18/19.
//

#ifndef GIE_TARGETEXPORTIMAGENODE_H
#define GIE_TARGETEXPORTIMAGENODE_H

#include <QLineEdit>
#include "src/widgets/imageviewer/imageviewer.h"
#include <QDockWidget>

#include "src/nodeeditor/DisplayNode.h"

class TargetExportImageNode: public DisplayNode
{
    Q_OBJECT
public:
    TargetExportImageNode():
            m_targetNameEdit{new QLineEdit()},
            m_id{QUuid::createUuid()},
            m_dock{new QDockWidget("Image Preview")},
            m_imageViewer{new ImageViewer(m_dock)}
    {
        m_targetNameEdit->setText("insert target name");
        connect(m_targetNameEdit, &QLineEdit::textEdited, this, &TargetExportImageNode::onTargetNameChanged);
        m_dock->setWidget(m_imageViewer);
        m_dock->setMinimumSize(100, 100);
    }

    virtual ~TargetExportImageNode() override = default;

public:
    QString caption() const override { return QString("TargetExport"); }
    bool captionVisible() const override { return true; }
    QString name() const override { return QString("TargetExportImage"); }

public:
    QJsonObject save() const override
    {
        QJsonObject modelJson = NodeDataModel::save();

        if(m_targetName != "")
            modelJson["target_name"] = m_targetName;

        return modelJson;
    }

    void restore(const QJsonObject& p) override
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

    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override
    {
        return ImageTypeData().type();
    }

    QWidget* embeddedWidget() override { return m_targetNameEdit; }

    const QString& getTargetName() const { return m_targetName; }
    QDockWidget* dockWidget() { return m_dock; }

    const QImage& getImage() const { return m_image; }

public Q_SLOTS:
    void displayData(Data data) override
    {
        const auto& image = std::get<Image>(data);

        m_image = QImage(
                image.width(),
                image.height(),
                QImage::Format_RGB888
        );

        for(int i = 0; i < static_cast<int>(image.height()); i++)
        {
            for(int j = 0; j < static_cast<int>(image.width()); j++)
            {
                Color color = image.pixelAt(i, j);
                m_image.setPixel(j, i, QColor{color.r, color.g, color.b}.rgb());
            }
        }

        m_imageViewer->setImage(m_image);
    }

Q_SIGNALS:
    void targetNameChanged(const QUuid& id, const QString&);

private Q_SLOTS:
    void onTargetNameChanged(const QString& name)
    {
        m_targetName = name;
        m_dock->setWindowTitle(name);
        Q_EMIT targetNameChanged(m_id, name);
    }

private:
    QLineEdit* m_targetNameEdit;
    QString m_targetName;
    QUuid m_id;

    QImage m_image;

    QDockWidget* m_dock;
    ImageViewer* m_imageViewer;
};

#endif //GIE_TARGETEXPORTIMAGENODE_H
