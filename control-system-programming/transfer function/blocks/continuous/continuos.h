#ifndef CONTINUOS_H
#define CONTINUOS_H
#include "blocks/integrator/integrator.h"

class continuos
{
public:
    continuos(float init1, float init2, float c);
    float update(float dt);
    float getState() const;

private:
    float state1;
    float state2;
    float c;
    integrator *intgr1;
    integrator *intgr2;
};

#endif // CONTINUOS_H
