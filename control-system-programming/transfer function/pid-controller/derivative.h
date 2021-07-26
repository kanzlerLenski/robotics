#ifndef DERIVATIVE_H
#define DERIVATIVE_H
#include "integrator.h"


class derivative
{
public:
    derivative();
    float update(float u);
    float getState() const;

private:
    float A = -53.86;
    float B = 64;
    float C = -45.33;
    float D = 53.86;
    float dt = 0.02;
    float x_dot = 0;
    float x;
    float y;
    integrator *integrator_4;
};

#endif // DERIVATIVE_H
