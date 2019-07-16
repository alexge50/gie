//
// Created by alex on 5/25/19.
//

#ifndef GUI_INTEGERSOURCEDATAMODEL_H
#define GUI_INTEGERSOURCEDATAMODEL_H

#include <QtCore/QObject>
#include <QtWidgets/QLineEdit>

#include <nodes/NodeDataModel>

#include "src/gie/types/TypeData.h"
#include "GieSourceDataModel.h"

class IntegerSourceDataModel: public GieSourceDataModel
{
Q_OBJECT

public:
    IntegerSourceDataModel();

    ~IntegerSourceDataModel() override {}

public:
    QString caption() const override { return QStringLiteral("integer source"); }
    bool captionVisible() const override { return false; }
    QString name() const override { return QStringLiteral("IntegerSource"); }

public:
    QJsonObject save() const override;
    void restore(QJsonObject const &p) override;

public:
    unsigned int nPorts(QtNodes::PortType portType) const override;
    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;

    QWidget* embeddedWidget() override { return m_lineEdit; }

private Q_SLOTS:
    void onTextEdited(QString const &string);

private:
    QLineEdit * m_lineEdit;
};

#endif //GUI_INTEGERSOURCEDATAMODEL_H
