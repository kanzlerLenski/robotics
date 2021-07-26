#include "continuos.h"
#include "../integrator/integrator.h"

continuos::continuos(float init_state1, float init_state2, float c)
{
    intgr1 = new integrator(init_state1);
    intgr2 = new integrator(init_state2);
    this-> c = c;
}

float continuos::update(float dt)
{
    state1 = intgr1->getState();
    state2 = intgr2->getState();
    intgr1->update(c * state2, dt);
    intgr2->update(state1, dt);
    return state2;
}

float continuos::getState() const
{
    return state2;
};

