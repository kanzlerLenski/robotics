#include "plant.h"

plant::plant(float a11, float a12, float a13, float a21, float a22, float a23, float a31, float a32, float a33, float b1, float b2, float b3, float c1, float c2, float c3, float d, float x1_dot, float x2_dot, float x3_dot)
{
    this -> a11 = a11;
    this -> a12 = a12;
    this -> a13 = a13;
    this -> a21 = a21;
    this -> a22 = a22;
    this -> a23 = a23;
    this -> a31 = a31;
    this -> a32 = a32;
    this -> a33 = a33;
    this -> b1 = b1;
    this -> b2 = b2;
    this -> b3 = b3;
    this -> c1 = c1;
    this -> c2 = c2;
    this -> c3 = c3;
    this -> d = d;
    intgr1 = new integrator(x1_dot);
    intgr2 = new integrator(x2_dot);
    intgr3 = new integrator(x3_dot);
}

float plant::update(float u, float dt)
{
    x1 = intgr1->getState(); // x1_dot
    x2 = intgr2->getState(); // x2_dot
    x3 = intgr3->getState(); // x3_dot
    intgr2->update(a21*x1 + a22*x2 + a23*x3 + b2*u, dt);
    intgr1->update(a11*x1 + a12*x2 + a13*x3 + b1*u, dt);
    intgr3->update(a31*x1 + a32*x2 + a33*x3 + b3*u, dt);
    y = c1*x1 + c2*x2 + c3*x3 + d*u;
    return y;
}

float plant::getState() const
{
    return y;
};

