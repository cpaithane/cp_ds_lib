#include "../include/header.h"

/*
 * This function compares integer data and returns 
 * 0 if both are equal
 * 1 if data1 > data2
 * -1 if data1 < data2
 */
int common_int_data_compare(const void *data1, const void *data2)
{

	int ret_val = IDENTICAL;
	int val1 = *((int *)data1);
	int val2 = *((int *)data2);

	if (val1 < val2)
	{
		ret_val = FIRST_LESS;
	}
	else if (val1 > val2)
	{
		ret_val = FIRST_GREATER;
	}
	
	return ret_val;

}

