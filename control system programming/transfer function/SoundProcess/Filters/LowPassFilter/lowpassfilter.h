#ifndef LOWPASSFILTER_H
#define LOWPASSFILTER_H
#include "Filters/AbstractFilter/abstractfilter.h"


class LowPassFilter : public AbstractFilter
{
public:
    LowPassFilter(float a1, float b1, float c1, float x1k);
    explicit LowPassFilter();
    virtual float update(float input) override;
private:
    float a1;
    float b1;
    float c1;
    float x1k;
    float x1k_1;
};

#endif // LOWPASSFILTER_H
