#ifndef INTEGRATOR_H
#define INTEGRATOR_H


class integrator
{
public:
    integrator(float init);
    float update(float input, float dt);
    float getState() const;

private:
    float state;
    float prev_in = 0;
};

#endif // INTEGRATOR_H
