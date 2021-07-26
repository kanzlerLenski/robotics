#include "discrete_continuous.h"

discrete_continuous::discrete_continuous(float a11, float a12, float a21, float a22, float b1, float b2, float c1, float c2, float d, float x1k, float x2k)
{
    this -> a11 = a11;
    this -> a12 = a12;
    this -> a21 = a21;
    this -> a22 = a22;
    this -> b1 = b1;
    this -> b2 = b2;
    this -> c1 = c1;
    this -> c2 = c2;
    this -> d = d;
    this -> x1k = x1k;
    this -> x2k = x2k;

}

float discrete_continuous::update(float uk)
{
    x1k_1 = a11*x1k + a12*x2k + b1*uk;
    x2k_1 = a21*x1k + a22*x2k + b2*uk;
    y = c1*x1k + c2*x2k + d*uk;;
    return y;
}

float discrete_continuous::getState() const
{
    return y;
};
