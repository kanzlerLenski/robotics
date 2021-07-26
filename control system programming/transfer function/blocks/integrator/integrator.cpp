#include "integrator.h"

integrator::integrator(float init_state)
{
    state = init_state;
}

float integrator::update(float input, float dt)
{
    state = state + (prev_in + input) * dt / 2;
    prev_in = input;
    return state;
}

float integrator::getState() const
{
    return state;
};

