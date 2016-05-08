#include "heap.h"

/*
 * This test case returns 3 minimum and maximum elements from max-heap and min-heap.
 */
int heap_test_case1()
{

	bool min_heap_type = HEAP_TYPE_MIN;
	bool max_heap_type = HEAP_TYPE_MAX;
	int rc = EOK;
	int min;
	int max;
	heap_st *min_heap = NULL;
	heap_st *max_heap = NULL;
	int data[9] = {50, 100, 200, 10, 20, 30, 70, 90, 110};
	int i;

	min_heap = heap_allocate_heap(sizeof(int), min_heap_type);
	CHECK_RC_ASSERT((min_heap == NULL), 0);

	max_heap = heap_allocate_heap(sizeof(int), max_heap_type);
	CHECK_RC_ASSERT((max_heap == NULL), 0);

	/*
	 * Insert 9 elements inside heaps.
	 */
	for (i = 0; i < 9; i++)
	{

		rc = heap_insert(min_heap,
			    &data[i],
			    sizeof(int),
			    min_heap_type,
			    common_int_data_compare);
		CHECK_RC_ASSERT(rc, EOK);

		rc = heap_insert(max_heap,
			    &data[i],
			    sizeof(int),
			    max_heap_type,
			    common_int_data_compare);
		CHECK_RC_ASSERT(rc, EOK);

	}

	/*
	 * Print both the heaps.
	 */
	printf("Min Heap = \n");
	rc = heap_print_heap(min_heap, heap_print_int_node);
	CHECK_RC_ASSERT(rc, EOK);
	printf("\n");

	printf("Max Heap = \n");
	rc = heap_print_heap(max_heap, heap_print_int_node);
	CHECK_RC_ASSERT(rc, EOK);
	printf("\n");

	/*
	 * Print 9 minimum elements from heap.
	 */
	printf("9 minimum elements = \n");
	for (i = 0; i < 9; i++)
	{

		heap_remove(min_heap,
			    &min,
			    sizeof(int),
			    HEAP_TYPE_MIN,
			    common_int_data_compare);
		heap_print_int_node(&min);

	}

	/*
	 * Print 9 maximum elements from heap.
	 */
	printf("\n9 maximum elements = \n");
	for (i = 0; i < 9; i++)
	{

		heap_remove(max_heap,
			    &max,
			    sizeof(int),
			    HEAP_TYPE_MAX,
			    common_int_data_compare);
		heap_print_int_node(&max);

	}

	rc = heap_deallocate_heap(min_heap);
	CHECK_RC_ASSERT(rc, EOK);
	rc = heap_deallocate_heap(max_heap);
	CHECK_RC_ASSERT(rc, EOK);

	return rc;

}

/*
 * This is a workhorse function for execution of heap test case(s).
 */
int heap_tc_execute()
{

	int rc = EOK;
	rc = heap_test_case1();

	return rc;

}

