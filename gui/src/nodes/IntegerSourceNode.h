//
// Created by alex on 7/18/19.
//

#ifndef GIE_INTEGERSOURCENODE_H
#define GIE_INTEGERSOURCENODE_H

#include <QLabel>
#include <QValidator>
#include "src/nodeeditor/SourceNode.h"

class IntegerSourceNode: public SourceNode
{
    Q_OBJECT

public:
    IntegerSourceNode():
        m_lineEdit(new QLineEdit())
    {
        m_lineEdit->setValidator(new QIntValidator{});
        m_lineEdit->setMaximumSize(m_lineEdit->sizeHint());

        connect(
                m_lineEdit, &QLineEdit::textChanged,
                this, &IntegerSourceNode::onTextEdited
        );

        m_lineEdit->setText("0");
    }

    ~IntegerSourceNode() override = default;

public:
    QString caption() const override { return QStringLiteral("IntegerSource"); }
    bool captionVisible() const override { return false; }
    QString name() const override { return QStringLiteral("IntegerSource"); }

public:
    QJsonObject save() const override
    {
        QJsonObject modelJson = NodeDataModel::save();

        modelJson["number"] = m_lineEdit->text();

        return modelJson;
    }

    void restore(QJsonObject const &p) override
    {
        QJsonValue v = p["number"];

        if (!v.isUndefined())
        {
            QString str = v.toString();

            bool ok{};
            str.toLongLong(&ok);

            if(ok)
            {
                m_lineEdit->setText(str);
            }
        }
    }

public:
    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override
    {
        return IntegerTypeData().type();
    }


    std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override
    {
        return std::make_shared<IntegerTypeData>();
    }

    QWidget* embeddedWidget() override { return m_lineEdit; }

    Data getData() override
    {
        return m_lineEdit->text().toLongLong();
    }

private Q_SLOTS:
    void onTextEdited(QString const &string)
    {
        bool ok;
        long long number = string.toLongLong(&ok);

        if(ok)
        {
            Q_EMIT dataChanged(number);
            this->ok();
        }
        else warning("");
    }

private:
    QLineEdit* m_lineEdit;
};

#endif //GIE_INTEGERSOURCENODE_H
