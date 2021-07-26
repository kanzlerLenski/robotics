#ifndef discrete_plant_H
#define discrete_plant_H


class discrete_plant
{
public:
    discrete_plant(float a11, float a12, float a13, float a21, float a22, float a23, float a31, float a32, float a33, float b1, float b2, float b3, float c1, float c2, float c3, float d, float x1k, float x2k, float x3k);
    float update(float uk);
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
    float x1k;
    float x1k_1;
    float x2k;
    float x2k_1;
    float x3k;
    float x3k_1;
    float y = 0;
};

#endif // discrete_plant_H
