#ifndef PARAMETERMODEL_H
#define PARAMETERMODEL_H

#include <QObject>
#include "paramter_type.h"

class ParameterModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ParameterTypeDef parameters READ parameters WRITE setParameters NOTIFY parametersChanged)
public:
    explicit ParameterModel(QObject *parent = nullptr);

    ParameterTypeDef parameters() const{
        return parameters_;
    }

    void setParameters(const ParameterTypeDef& parameters){
        if(!(parameters_ == parameters)){
            this->parameters_=parameters;
            emit parametersChanged(parameters);
        }
    }

    virtual ~ParameterModel() {};

private:
    ParameterTypeDef parameters_;
signals:
    void parametersChanged(const ParameterTypeDef& parameters);


};

#endif // PARAMETERMODEL_H
