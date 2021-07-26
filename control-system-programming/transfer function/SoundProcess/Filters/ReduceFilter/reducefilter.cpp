#include "reducefilter.h"

ReduceFilter::ReduceFilter() : AbstractFilter()
{

}

float ReduceFilter::update(float input)
{
	m_output = input / 2;
	return m_output;
}
