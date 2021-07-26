#include "pid_controller.h"

PID_controller::PID_controller(QObject *parent) : QObject(parent)
{
    plant_data = new QFile(QDateTime::currentDateTime().toString("hh_MM_ss"));
    PID_data = new QFile(QDateTime::currentDateTime().toString("hh_MM_ss") + "_check");

    if (!plant_data->open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to open plant_data.";
    }

    if (!PID_data->open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to open PID_data.";
    }

    plant_data_steam = new QTextStream(plant_data);
    PID_data_steam = new QTextStream(PID_data);

    lowpassfilter = new LowPassFilter();
    integrator_2 = new integrator(0);
    derivative_1 = new derivative();

}

PID_controller::~PID_controller()
{
    plant_data->close();

    delete plant_data;
    delete plant_data_steam;

    PID_data->close();

    delete PID_data;
    delete PID_data_steam;
}

void PID_controller::get(QByteArray outputBytes)
{
    qDebug() << "Signal detected: " << outputBytes.toHex(' ');

    quint8 sum = 0;

    for (quint8 i = 0; i < outputBytes.size(); ++i) {
            sum += (quint8)outputBytes[i];
    }

    if (sum == 0xFF) {
        float outputSignal;
        float referenceSignal;
        memcpy(&outputSignal, outputBytes.data() + 2, 4);
        memcpy(&referenceSignal, outputBytes.data() + 6, 4);
        qDebug() << "Current position of an object: " << outputSignal;
        qDebug() << "Required position of an object: " << referenceSignal;
        //plant_data->write((char*)&outputSignal, sizeof (float));
        //plant_data->write((char*)&referenceSignal, sizeof (float));
        *plant_data_steam << outputSignal << " " << referenceSignal << "\n";
        //emit generatedReference(referenceSignal);
        //emit generatedInput(referenceSignal);
        //main(outputSignal, referenceSignal);
        check(outputSignal);
    }

     else {
            qDebug() << "Signal contains broken bytes. Ignored.";
    }

}

void PID_controller::main(float outputSignal, float referenceSignal)
{
    float filtered_outputSignal = lowpassfilter->update(outputSignal);

    float error = referenceSignal - filtered_outputSignal;

    float dt = 0.02;

    integrator_2->update(error, dt);
    derivative_1->update(error);

    float P = 15.0612716614161;
    float I = 12.2669420894523;
    float D = 2.12067107547282;

    float PID_P = P * error;
    float PID_I = I * integrator_2->getState();
    float PID_D = D * derivative_1->getState();

    float PID_control = PID_P + PID_I + PID_D;

    emit generatedReference(referenceSignal);
    emit generatedInput(PID_control);

    *PID_data_steam << outputSignal << " " << referenceSignal << "\n";

}

int i = 0;

void PID_controller::check(float outputSignal)
{
    i++;
    float t = i * 0.02;
    float referencesignal = 70 - 10 * std::exp(-0.1*t) * std::sin(t/2*(std::pow((-(t - 2)), 2) + 60*t)/100);
    main(outputSignal, referencesignal);

}
