#ifndef LOWPASSFILTER_H
#define LOWPASSFILTER_H
#include "integrator.h"
#include <QtMath>

class LowPassFilter
{
public:
    LowPassFilter();
    float update(float u);

private:
    integrator *integrator_1;
    float y_dot = 0;
    float dt = 0.02;
    float f = 50 * 0.05 * 2 * M_PI;
};

#endif // LOWPASSFILTER_H
