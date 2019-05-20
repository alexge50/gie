//
// Created by alex on 5/20/19.
//

#ifndef GUI_NUMBERSOURCEDATAMODEL_H
#define GUI_NUMBERSOURCEDATAMODEL_H

#include <QtCore/QObject>
#include <QtWidgets/QLineEdit>

#include <nodes/NodeDataModel>

#include "../NumberData.h"

class NumberSourceDataModel: public QtNodes::NodeDataModel
{
    Q_OBJECT

public:
    NumberSourceDataModel();

    ~NumberSourceDataModel() override {}

public:
    QString caption() const override { return QStringLiteral("number source"); }
    bool captionVisible() const override { return false; }
    QString name() const override { return QStringLiteral("NumberSource"); }

public:
    QJsonObject save() const override;
    void restore(QJsonObject const &p) override;

public:
    unsigned int nPorts(QtNodes::PortType portType) const override;
    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;
    void setInData(std::shared_ptr<QtNodes::NodeData>, int) override { }

    QWidget* embeddedWidget() override { return m_lineEdit; }

private Q_SLOTS:
    void onTextEdited(QString const &string);

private:
    std::shared_ptr<NumberData> m_data;
    QLineEdit * m_lineEdit;
};


#endif //GUI_NUMBERSOURCEDATAMODEL_H
