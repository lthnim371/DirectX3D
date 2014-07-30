#include "stdafx.h"
#include "number.h"

short common::GetSigh(const float& number)
{
	if(number > MATH_EPSILON)
		return 1;
	else if(number < -(MATH_EPSILON) )
		return -1;
	else
		return 0;
}