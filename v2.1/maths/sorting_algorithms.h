#pragma once

#ifndef __SORTING_ALGORITHMS__
#define __SORTING_ALGORITHMS__

#include "../memory/memutils.h"

/*
	min() : Returns the smallest (min) value of values

*/
float min(float* values, ulong values_length)
{
	float candidate = values[0];
	for(ulong i = 1 ; i < values_length ; i++)
	{
		if(values[i] < candidate)
		{
			candidate = values[i];
		}
	}
	return candidate;
}

/*
	max() : Returns the biggest (max) value of values

*/
float max(float* values, ulong values_length)
{
	float candidate = values[0];
	for(ulong i = 1 ; i < values_length ; i++)
	{
		if(values[i] > candidate)
		{
			candidate = values[i];
		}
	}
	return candidate;
}

#endif