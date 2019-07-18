//
// Created by alex on 7/18/19.
//

#ifndef GIE_STRINGSOURCENODE_H
#define GIE_STRINGSOURCENODE_H

#include <QLabel>
#include "src/nodeeditor/SourceNode.h"

class StringSourceNode: public SourceNode
{
Q_OBJECT

public:
    StringSourceNode():
        m_lineEdit(new QLineEdit())
    {
        m_lineEdit->setMaximumSize(m_lineEdit->sizeHint());

        connect(
                m_lineEdit, &QLineEdit::textChanged,
                this, &StringSourceNode::onTextEdited
        );

        m_lineEdit->setText("string");
    }

    ~StringSourceNode() override {}

public:
    QString caption() const override { return QStringLiteral("string source"); }
    bool captionVisible() const override { return false; }
    QString name() const override { return QStringLiteral("StringSource"); }

public:
    QJsonObject save() const override
    {
        QJsonObject modelJson = NodeDataModel::save();
        modelJson["string"] = m_lineEdit->text();
        return modelJson;
    }

    void restore(QJsonObject const &p) override
    {
        QJsonValue v = p["string"];

        if (!v.isUndefined())
        {
            QString str = v.toString();
            m_lineEdit->setText(str);
        }
    }

public:
    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override
    {
        return StringTypeData().type();
    }

    std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override
    {
        return std::make_shared<StringTypeData>();
    }

    QWidget* embeddedWidget() override { return m_lineEdit; }

    Data getData() override
    {
        return m_lineEdit->text().toStdString();
    }

private Q_SLOTS:
    void onTextEdited(QString const &string)
    {
        Q_EMIT dataChanged(string.toStdString());
        ok();
    }

private:
    QLineEdit * m_lineEdit;
};


#endif //GIE_STRINGSOURCENODE_H
