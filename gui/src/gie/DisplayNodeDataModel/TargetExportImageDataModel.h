//
// Created by alex on 5/28/19.
//

#ifndef GUI_TARGETEXPORTIMAGEDATAMODEL_H
#define GUI_TARGETEXPORTIMAGEDATAMODEL_H

#include <nodes/NodeDataModel>
#include <QLineEdit>

class TargetExportImageDataModel: public QtNodes::NodeDataModel
{
    Q_OBJECT
public:
    TargetExportImageDataModel();
    virtual ~TargetExportImageDataModel() override = default;

public:
    QString caption() const override { return QString("export target"); }
    bool captionVisible() const override { return true; }
    QString name() const override { return QString("TargetExportImage"); }

public:
    QJsonObject save() const override;
    void restore(const QJsonObject& p) override;

    unsigned int nPorts(QtNodes::PortType portType) const override;
    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex portIndex) override { return {}; }
    void setInData(std::shared_ptr<QtNodes::NodeData>, QtNodes::PortIndex portIndex) override;

    QWidget* embeddedWidget() override { return m_targetNameEdit; }

    QtNodes::NodeValidationState validationState() const override { return modelValidationState; }
    QString validationMessage() const override { return modelValidationError; }

    const QString& getTargetName() const { return m_targetName; }
    QString getId() const { return m_id.toString(); };

Q_SIGNALS:
    void targetNameChanged(const QUuid& id, const QString&);

private Q_SLOTS:
    void onTargetNameChanged(const QString&);

private:
    QtNodes::NodeValidationState modelValidationState = QtNodes::NodeValidationState::Warning;
    QString modelValidationError = QString("Missing or incorrect inputs");

    QLineEdit* m_targetNameEdit;
    QString m_targetName;
    QUuid m_id;
};


#endif //GUI_TARGETEXPORTIMAGEDATAMODEL_H
