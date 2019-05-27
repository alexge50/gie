//
// Created by alex on 5/28/19.
//

#include "TargetExportImageDataModel.h"
#include "src/gie/types/ImageData.h"

TargetExportImageDataModel::TargetExportImageDataModel():
        m_targetNameEdit{new QLineEdit()},
        m_id{QUuid::createUuid()}
{
    m_targetNameEdit->setText("insert target name");
    connect(m_targetNameEdit, &QLineEdit::textEdited, this, &TargetExportImageDataModel::onTargetNameChanged);
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

void TargetExportImageDataModel::setInData(std::shared_ptr<QtNodes::NodeData> data, QtNodes::PortIndex)
{
    if(!data)
    {
        modelValidationState = QtNodes::NodeValidationState::Valid;
        modelValidationError = QStringLiteral("Missing or incorrect inputs");
    }
    else
    {
        modelValidationState = QtNodes::NodeValidationState::Valid;
        modelValidationError = QString();
    }
}

void TargetExportImageDataModel::onTargetNameChanged(const QString& name)
{
    m_targetName = name;
    Q_EMIT targetNameChanged(m_id, name);
}