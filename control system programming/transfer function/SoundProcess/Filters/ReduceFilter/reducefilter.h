#ifndef REDUCEFILTER_H
#define REDUCEFILTER_H

#include "Filters/AbstractFilter/abstractfilter.h"

class ReduceFilter : public AbstractFilter
{
public:
	explicit ReduceFilter();

	virtual float update(float input) override;
};

#endif // REDUCEFILTER_H
