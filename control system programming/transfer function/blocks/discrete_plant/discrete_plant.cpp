#include "discrete_plant.h"

discrete_plant::discrete_plant(float a11, float a12, float a13, float a21, float a22, float a23, float a31, float a32, float a33, float b1, float b2, float b3, float c1, float c2, float c3, float d, float x1k, float x2k, float x3k)
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
    this -> x1k = x1k;
    this -> x2k = x2k;
    this -> x3k = x3k;
}

float discrete_plant::update(float uk)
{
    x1k_1 = a11*x1k + a12*x2k + a13*x3k + b1*uk;
    x2k_1 = a21*x1k + a22*x2k + a23*x3k + b2*uk;
    x3k_1 = a31*x1k + a32*x2k + a33*x3k + b3*uk;
    y = c1*x1k + c2*x2k + c3*x3k + d*uk;;
    return y;
}

float discrete_plant::getState() const
{
    return y;
};
