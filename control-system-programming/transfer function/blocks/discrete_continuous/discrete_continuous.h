#ifndef DISCRETE_CONTINUOUS_H
#define DISCRETE_CONTINUOUS_H


class discrete_continuous
{
public:
    discrete_continuous(float a11, float a12, float a21, float a22, float b1, float b2, float c1, float c2, float d, float x1k, float x2k);
    float update(float uk);
    float getState() const;

private:
    float a11;
    float a12;
    float a21;
    float a22;
    float b1;
    float b2;
    float c1;
    float c2;
    float d;
    float x1k;
    float x1k_1;
    float x2k;
    float x2k_1;
    float y = 0;
};

#endif // DISCRETE_CONTINUOUS_H
