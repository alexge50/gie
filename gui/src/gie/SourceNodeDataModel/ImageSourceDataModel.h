//
// Created by alex on 5/25/19.
//

#ifndef GUI_IMAGESOURCEDATAMODEL_H
#define GUI_IMAGESOURCEDATAMODEL_H

#include <nodes/NodeDataModel>

#include "src/filepicker/filepicker.h"
#include "src/gie/types/ImageData.h"

class ImageSourceDataModel: public QtNodes::NodeDataModel
{
    Q_OBJECT
public:
    ImageSourceDataModel();
    ~ImageSourceDataModel() override = default;

public:
    QString caption() const override { return QStringLiteral("image source"); }
    bool captionVisible() const override { return false; }
    QString name() const override { return QStringLiteral("ImageSource"); }

public:
    QJsonObject save() const override;
    void restore(QJsonObject const &p) override;

public:
    unsigned int nPorts(QtNodes::PortType portType) const override;
    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
    void setInData(std::shared_ptr<QtNodes::NodeData>, int) override { }

    QWidget* embeddedWidget() override { return m_filePicker; }

Q_SIGNALS:
    void onValueChanged(std::shared_ptr<QtNodes::NodeData>);

private Q_SLOTS:
    void onFileChanged(QString);

private:
    FilePicker* m_filePicker;
    std::shared_ptr<ImageData> m_data;
    QString m_filename;
};


#endif //GUI_IMAGESOURCEDATAMODEL_H
