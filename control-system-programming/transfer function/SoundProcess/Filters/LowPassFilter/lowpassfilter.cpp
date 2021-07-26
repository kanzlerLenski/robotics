#include "lowpassfilter.h"

LowPassFilter::LowPassFilter(float a1, float b1, float c1, float x1k)
{
    this -> a1 = a1;
    this -> b1 = b1;
    this -> c1 = c1;
    this -> x1k = x1k;
}
float LowPassFilter::update(float input)
{
    m_output = c1*x1k;
    x1k =  a1*x1k + b1*input;

    return m_output;
}
