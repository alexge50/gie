//
// Created by alex on 7/18/19.
//

#ifndef GIE_NUMBERSOURCENODE_H
#define GIE_NUMBERSOURCENODE_H

#include "src/nodeeditor/SourceNode.h"

class NumberSourceNode: public SourceNode
{
Q_OBJECT

public:
    NumberSourceNode():
        m_lineEdit(new QLineEdit())
    {
        m_lineEdit->setValidator(new QDoubleValidator());
        m_lineEdit->setMaximumSize(m_lineEdit->sizeHint());

        connect(
                m_lineEdit, &QLineEdit::textChanged,
                this, &NumberSourceNode::onTextEdited
        );

        m_lineEdit->setText("0.0");
    }

    ~NumberSourceNode() override {}

public:
    QString caption() const override { return QStringLiteral("NumberSource"); }
    bool captionVisible() const override { return false; }
    QString name() const override { return QStringLiteral("NumberSource"); }

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
            str.toDouble(&ok);
            if(ok)
                m_lineEdit->setText(str);
        }
    }

public:
    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override
    {
        return NumberTypeData().type();
    }

    std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override
    {
        return std::make_shared<NumberTypeData>();
    }

    QWidget* embeddedWidget() override { return m_lineEdit; }

    Data getData() override
    {
        return m_lineEdit->text().toDouble();
    }

private Q_SLOTS:
    void onTextEdited(QString const &string)
    {
        bool ok;
        double number = string.toDouble(&ok);

        if(ok)
        {
            Q_EMIT dataChanged(number);
            this->ok();
        }
        else warning("Missing or incorrect number of inputs");
    }

private:
    QLineEdit * m_lineEdit;
};

#endif //GIE_NUMBERSOURCENODE_H
