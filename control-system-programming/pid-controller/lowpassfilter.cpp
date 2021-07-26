#include "lowpassfilter.h"

LowPassFilter::LowPassFilter()
{
    integrator_1 = new integrator(y_dot);
}

float LowPassFilter::update(float u)
{ 
    float tau = 1/f;
    float dt = 0.02;
    float y = integrator_1->getState();
    y_dot = (1/tau * u) - (1/tau * y);
    integrator_1->update(y_dot, dt);

    return y;
}
