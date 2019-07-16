//
// Created by alex on 7/16/19.
//

#ifndef GIE_GIEDISPLAYDATAMODEL_H
#define GIE_GIEDISPLAYDATAMODEL_H

#include <QtCore/QObject>
#include <nodes/NodeDataModel>
#include <src/gie/types/TypeData.h>

class GieDisplayDataModel: public QtNodes::NodeDataModel
{
    Q_OBJECT
public:
    std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex portIndex) override { return {}; }

    unsigned int nPorts(QtNodes::PortType portType) const override
    {
        if(portType == QtNodes::PortType::In)
            return 1;
        return 0;
    }

    void setInData(std::shared_ptr<QtNodes::NodeData> input, QtNodes::PortIndex portIndex) override
    {
        if(input)
        {
            modelValidationState = QtNodes::NodeValidationState::Valid;
            modelValidationError = QString();
        }
        else
        {
            modelValidationState = QtNodes::NodeValidationState::Valid;
            modelValidationError = QStringLiteral("Missing or incorrect inputs");
        }
    }

    QtNodes::NodeValidationState validationState() const override { return modelValidationState; }
    QString validationMessage() const override { return modelValidationError; }

public Q_SLOTS:
    virtual void displayData(Data data) = 0;

private:
    QtNodes::NodeValidationState modelValidationState = QtNodes::NodeValidationState::Warning;
    QString modelValidationError = QString("Missing or incorrect inputs");
};

#endif //GIE_GIEDISPLAYDATAMODEL_H
