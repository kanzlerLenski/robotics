#include "abstractfilter.h"

AbstractFilter::AbstractFilter()
{
	m_output = 0;
}

float AbstractFilter::getOutput() const
{
	return m_output;
}
