#ifndef PLANT_H
#define PLANT_H
#include "blocks/integrator/integrator.h"

class plant
{
public:
    plant(float a11, float a12, float a13, float a21, float a22, float a23, float a31, float a32, float a33, float b1, float b2, float b3, float c1, float c2, float c3, float d, float x1_dot, float x2_dot, float x3_dot);
    float update(float u, float dt);
    float getState() const;

private:
    float a11;
    float a12;
    float a13;
    float a21;
    float a22;
    float a23;
    float a31;
    float a32;
    float a33;
    float b1;
    float b2;
    float b3;
    float c1;
    float c2;
    float c3;
    float d;
    float x1;
    float x1_dot;
    float x2;
    float x2_dot;
    float x3;
    float x3_dot;
    float y = 0;
    integrator *intgr1;
    integrator *intgr2;
    integrator *intgr3;
};


#endif // PLANT_H
