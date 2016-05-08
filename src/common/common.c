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

/*
 * This function swaps data pointers.
 */
void swap(void *data1, void *data2)
{

	void *tmp = data1;
	data1 = data2;
	data2 = tmp;

}

/*
 * This function finds out greatest from three elements.
 */
void *find_greatest_from_three(
			    void *root,
			    void * left_child,
			    void *right_child,
			    common_data_compare_t compare)
{

	int rc;
	void *greatest = left_child;

	rc = compare(root, left_child);
	if (rc == FIRST_GREATER)
	{
		greatest = root;
	}

	rc = compare(greatest, right_child);
	if (rc == FIRST_LESS)
	{
		greatest = right_child;
	}

	return greatest;

}

/*
 * This function finds out smallest from three elements.
 */
void *find_smallest_from_three(
			    void *root,
			    void * left_child,
			    void *right_child,
			    common_data_compare_t compare)
{

	int rc;
	void *smallest = left_child;

	rc = compare(root, left_child);
	if (rc == FIRST_LESS)
	{
		smallest = root;
	}

	rc = compare(smallest, right_child);
	if (rc == FIRST_GREATER)
	{
		smallest = right_child;
	}

	return smallest;

}

