//
// Created by alex on 7/17/19.
//

#ifndef GIE_STRINGDISPLAYNODE_H
#define GIE_STRINGDISPLAYNODE_H

#include "src/nodeeditor/DisplayNode.h"
#include <QLabel>

class StringDisplayNode: public DisplayNode
{
Q_OBJECT
public:
    StringDisplayNode():
            m_label{new QLabel()}
    {
        m_label->setMargin(3);
    }

    virtual ~StringDisplayNode() override = default;

public:
    QString caption() const override { return QString("ColorDisplay"); }
    bool captionVisible() const override { return false; }
    QString name() const override { return QString("ColorDisplay"); }

public:
    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override
    {
        return StringTypeData().type();
    }

    QWidget* embeddedWidget() override { return m_label; }

public Q_SLOTS:
    void displayData(Data data) override
    {
        const std::string& text = std::get<std::string>(data);
		m_label->setText(QString::fromStdString(text));
    };

private:
    QLabel* m_label;
};

#endif //GIE_STRINGDISPLAYNODE_H
