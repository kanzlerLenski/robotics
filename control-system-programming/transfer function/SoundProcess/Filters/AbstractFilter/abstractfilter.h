#ifndef ABSTRACTFILTER_H
#define ABSTRACTFILTER_H


class AbstractFilter
{
public:
	explicit AbstractFilter();

	virtual float update(float update) = 0;
	float getOutput() const;

protected:
	float m_output;
};

#endif // ABSTRACTFILTER_H
