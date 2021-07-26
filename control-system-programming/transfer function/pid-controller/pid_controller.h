#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include "integrator.h"
#include "lowpassfilter.h"
#include "derivative.h"

class PID_controller : public QObject
{
    Q_OBJECT

public:
    explicit PID_controller(QObject *parent = nullptr);
    ~PID_controller();

signals:
    void generatedInput(float value);
    void generatedReference(float value);

public slots:

    void get(QByteArray inputBytes);

private:
    QFile* plant_data;
    QTextStream* plant_data_steam;
    QFile* PID_data;
    QTextStream* PID_data_steam;
    LowPassFilter* lowpassfilter;
    integrator *integrator_2;
    derivative* derivative_1;

void main(float outputSignal, float referenceSignal);

void check(float outputSignal);

};

#endif // PID_CONTROLLER_H
