//
// Created by alex on 7/17/19.
//

#ifndef GIE_NUMBERDISPLAYNODE_H
#define GIE_NUMBERDISPLAYNODE_H

#include "src/nodeeditor/DisplayNode.h"
#include <QLabel>

class NumberDisplayNode: public DisplayNode
{
Q_OBJECT
public:
    NumberDisplayNode():
            m_label{new QLabel()}
    {
        m_label->setMargin(3);
    }

    virtual ~NumberDisplayNode() override = default;

public:
    QString caption() const override { return QString("ColorDisplay"); }
    bool captionVisible() const override { return false; }
    QString name() const override { return QString("ColorDisplay"); }

public:
    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override
    {
        return NumberTypeData().type();
    }

    QWidget* embeddedWidget() override { return m_label; }

public Q_SLOTS:
    void displayData(Data data) override
    {
        double number = std::get<long long int>(data);
		m_label->setText(QString::number(number));
    };

private:
    QLabel* m_label;
};

#endif //GIE_NUMBERDISPLAYNODE_H
