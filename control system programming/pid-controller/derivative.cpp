#include "derivative.h"

derivative::derivative()
{
    integrator_4 = new integrator(x_dot);

}

float derivative::update(float u)
{
    x = integrator_4->getState();
    integrator_4->update(A*x + B*u, dt);
    y = C*x + D*u;
    return y;
}

float derivative::getState() const
{
    return y;
}
