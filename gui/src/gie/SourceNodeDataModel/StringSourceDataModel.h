//
// Created by alex on 5/20/19.
//

#ifndef GUI_STRINGSOURCEDATAMODEL_H
#define GUI_STRINGSOURCEDATAMODEL_H

#include <QtCore/QObject>
#include <QtWidgets/QLineEdit>

#include <nodes/NodeDataModel>

#include "../StringData.h"

class StringSourceDataModel: public QtNodes::NodeDataModel
{
    Q_OBJECT

public:
    StringSourceDataModel();

    ~StringSourceDataModel() override {}

public:
    QString caption() const override { return QStringLiteral("string source"); }
    bool captionVisible() const override { return false; }
    QString name() const override { return QStringLiteral("StringSource"); }

public:
    QJsonObject save() const override;
    void restore(QJsonObject const &p) override;

public:
    unsigned int nPorts(QtNodes::PortType portType) const override;
    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
    void setInData(std::shared_ptr<QtNodes::NodeData>, int) override { }

    QWidget* embeddedWidget() override { return m_lineEdit; }

Q_SIGNALS:
    void onValueChanged(std::shared_ptr<QtNodes::NodeData>);

private Q_SLOTS:
    void onTextEdited(QString const &string);

private:
    std::shared_ptr<StringData> m_data;
    QLineEdit * m_lineEdit;
};



#endif //GUI_STRINGSOURCEDATAMODEL_H
