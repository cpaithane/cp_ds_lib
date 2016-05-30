#include "heap.h"

/*
 * This function allocates a heap data structure and initializes it with default
 * values. len is sizeof data stored inside node of heap.
 */
heap_st *heap_allocate_heap(size_t len, bool heap_type)
{

	int i;
	heap_st *heap = NULL;

	/*
	 * Allocate memory and initialize for main data structure.
	 */
	heap = malloc(sizeof(heap_st));
	if (heap == NULL)
	{
		return NULL;
	}
	memset(heap, 0, sizeof(heap_st));

	/*
	 * Allocate and initialize for data of heap.
	 */
	for (i = 0; i < HEAP_MAX_SIZE; i++)
	{

		heap->heap_data[i] = (void *)malloc(len);
		if (heap->heap_data[i] == NULL)
		{
			break;
		}
		memset(heap->heap_data[i], 0, len);

	}

	CHECK_RC_ASSERT(i, HEAP_MAX_SIZE);

	/*
	 * Initialize heap_size as -1.
	 */
	heap->heap_size = HEAP_MIN_SIZE;

	/*
	 * This implementation will allow to manage heap using the same data 
	 * structure and same code.
	 */
	heap->heap_type = heap_type;

	return heap;
}

/*
 * This function deallocates heap and associated data inside it.
 */
int heap_deallocate_heap(heap_st *heap)
{

	int rc = EOK;
	int i;

	/*
	 * Heap has some elements. So, return ENOTEMPTY from here.
	 */
	if (heap->heap_size != HEAP_MIN_SIZE)
	{

		rc = ENOTEMPTY;
		return rc;

	}

	for (i = 0; i < HEAP_MAX_SIZE; i++)
	{
		free(heap->heap_data[i]);
	}

	free(heap);
	return rc;

}

/*
 * This function returns index of parent
 */
int heap_get_parent(int i)
{
	return ((i - 1) / 2);
}

/*
 * This function return index of left child
 */
int heap_get_left_child(int i)
{
	return ((i * 2) + 1);
}

/*
 * This function return index of right child
 */
int heap_get_right_child(int i)
{
	return ((i * 2) + 2);
}

/*
 * This function runs basic validation on inputs while inserting data into heap.
 * Details are listed in the comment section.
 */
int heap_validate_input(heap_st *heap, bool type)
{

	int rc = EOK;

	/*
	 * Heap either should be of type min or max.
	 */
	if ((type != HEAP_TYPE_MIN) && (type != HEAP_TYPE_MAX))
	{
		rc = ENOTSUP;
	}

	/*
	 * Stroing the same type of heap in a data structure is allowed. However, 
	 * both not at the same time in the same heap_st. 
	 */
	if ((heap) && (heap->heap_size != HEAP_MIN_SIZE) &&
	    (heap->heap_type != type))
	{
		rc = ENOTSUP;
	}

	CHECK_RC_ASSERT(rc, EOK);

	if (heap->heap_size == (HEAP_MAX_SIZE - 1))
	{
		rc = EXFULL;
	}
	return rc;

}

/*
 * This function inserts an element into max-heap.
 */
int heap_insert_max(heap_st *heap,
		    void *data,
		    size_t len,
		    common_data_compare_t compare)
{

	int rc = EOK;
	int i = HEAP_MIN_SIZE;
	void *tmp;

	/*
	 * First increament heap_size to accomodate new data.
	 */
	i = heap->heap_size;
	(heap->heap_size)++;
	memcpy(heap->heap_data[i], data, len);

	while ((i != 0) && 
	compare(data, heap->heap_data[heap_get_parent(i)]) == FIRST_GREATER)
	{

		tmp = heap->heap_data[i];
		heap->heap_data[i] = heap->heap_data[heap_get_parent(i)];
		heap->heap_data[heap_get_parent(i)] = tmp;
		i = heap_get_parent(i);

	}

	return rc;

}

/*
 * This function inserts an element into min-heap.
 */
int heap_insert_min(heap_st *heap,
		    void *data,
		    size_t len,
		    common_data_compare_t compare)
{

	int rc = EOK;
	int i = HEAP_MIN_SIZE;
	void *tmp;

	/*
	 * First increament heap_size to accomodate new data.
	 */
	i = heap->heap_size;
	(heap->heap_size)++;
	memcpy(heap->heap_data[i], data, len);

	while ((i != 0) && 
		compare(data, heap->heap_data[heap_get_parent(i)]) == FIRST_LESS)
	{

		tmp = heap->heap_data[i];
		heap->heap_data[i] = heap->heap_data[heap_get_parent(i)];
		heap->heap_data[heap_get_parent(i)] = tmp;
		i = heap_get_parent(i);

	}

	return rc;

}

/*
 * This function inserts an element into heap. As heap is a complete binary tree, it
 * makes the perfect candidate to store the contents of heap in an array.
 */
int heap_insert(heap_st *heap,
                void *data,
                size_t len,
                bool heap_type,
                common_data_compare_t compare)
{

	int rc = EOK;

	rc = heap_validate_input(heap, heap_type);
	if (rc != EOK)
	{
		return rc;
	}

	if (heap_type == HEAP_TYPE_MIN)
	{
		rc = heap_insert_min(heap, data, len, compare);
	}
	else
	{
		rc = heap_insert_max(heap, data, len, compare);
	}

	return rc;

}

/*
 * This function checks left and right children's values with values stored at index.
 * If max-heap property is not satisfied, then swap the root and maximum of children.
 * Recurse this process.
 */
int heap_max_heapify(heap_st *heap, int index, common_data_compare_t compare)
{

	int rc = EOK;
	int left_idx, right_idx, greatest_idx;
	void *root, *left_child, *right_child, *greatest, *tmp;
	int right_invalid = 0;

	left_idx = heap_get_left_child(index);
	right_idx = heap_get_right_child(index);

	if ((left_idx >= heap->heap_size) && (right_idx >= heap->heap_size))
	{
		return rc;
	}

	root = heap->heap_data[index];
	left_child = heap->heap_data[left_idx];

	greatest = left_child;
	greatest_idx = left_idx;

	/*
	 * If right children is invalid, then check whether root and left are in min
	 * heap order. If they are, return from here.
	 */
	if (right_idx >= heap->heap_size)
	{

		right_invalid = 1;
		if (compare(root, greatest) == FIRST_GREATER)
		{
			return rc;
		}

	}

	if (right_invalid == 0)
	{

		right_child = heap->heap_data[right_idx];
	
		greatest = find_greatest_from_three(root,
						    left_child,
						    right_child,
						    compare);
		if (compare(greatest, root) == IDENTICAL)
		{
			greatest_idx = index;
		}
		else if (compare(greatest, left_child) == IDENTICAL)
		{
			greatest_idx = left_idx;
		}
		else if (compare(greatest, right_child) == IDENTICAL)
		{
			greatest_idx = right_idx;
		}

	}

	if (greatest_idx != index)
	{

		tmp = heap->heap_data[index];
		heap->heap_data[index] = heap->heap_data[greatest_idx];
		heap->heap_data[greatest_idx] = tmp;

		rc = heap_max_heapify(heap, greatest_idx, compare);

	}

	return rc;

}

/*
 * This function takes out an element from root of the heap.
 */
int heap_remove_max(heap_st *heap,
		    void *data,
		    size_t len,
		    common_data_compare_t compare)
{

	int rc = EOK;

	memcpy(data, heap->heap_data[HEAP_ROOT_LOC], len);

	/*
	 * Check for overlap.
	 */
	if (HEAP_ROOT_LOC != (heap->heap_size - 1))
	{

		memcpy(heap->heap_data[HEAP_ROOT_LOC],
			heap->heap_data[heap->heap_size - 1],
			len);

	}

	(heap->heap_size)--;

	rc = heap_max_heapify(heap, 0, compare);
	return rc;

}

/*
 * This function checks left and right children's values with values stored at index.
 * If min-heap property is not satisfied, then swap the root and minimum of children.
 * Recurse this process.
 */
int heap_min_heapify(heap_st *heap, int index, common_data_compare_t compare)
{

	int rc = EOK;
	int left_idx, right_idx, smallest_idx;
	void *root, *left_child, *right_child, *smallest, *tmp;
	int right_invalid = 0;

	left_idx = heap_get_left_child(index);
	right_idx = heap_get_right_child(index);

	/*
	 * If both the indices are invalid, then heapify has reached end of the
	 * subtree.
	 */
	if ((left_idx >= heap->heap_size) && (right_idx >= heap->heap_size))
	{
		return rc;
	}

	root = heap->heap_data[index];
	left_child = heap->heap_data[left_idx];
	smallest = left_child;
	smallest_idx = left_idx;

	/*
	 * If right children is invalid, then check whether root and left are in min
	 * heap order. If they are, return from here.
	 */
	if (right_idx >= heap->heap_size)
	{

		right_invalid = 1;
		if (compare(root, smallest) == FIRST_LESS)
		{
			return rc;
		}

	}

	if (right_invalid == 0)
	{

		right_child = heap->heap_data[right_idx];
	
		smallest = find_smallest_from_three(root,
						    left_child,
						    right_child,
						    compare);
		if (compare(smallest, root) == IDENTICAL)
		{
			smallest_idx = index;
		}
		else if (compare(smallest, left_child) == IDENTICAL)
		{
			smallest_idx = left_idx;
		}
		else if (compare(smallest, right_child) == IDENTICAL)
		{
			smallest_idx = right_idx;
		}

	}

	if (smallest_idx != index)
	{

		tmp = heap->heap_data[index];
		heap->heap_data[index] = heap->heap_data[smallest_idx];
		heap->heap_data[smallest_idx] = tmp;

		rc = heap_min_heapify(heap, smallest_idx, compare);

	}

	return rc;

}

/*
 * This function takes out an element from root of the heap.
 */
int heap_remove_min(heap_st *heap,
		    void *data,
		    size_t len,
		    common_data_compare_t compare)
{

	int rc = EOK;

	memcpy(data, heap->heap_data[HEAP_ROOT_LOC], len);

	/*
	 * Check for overlap.
	 */
	if (HEAP_ROOT_LOC != (heap->heap_size - 1))
	{

		memcpy(heap->heap_data[HEAP_ROOT_LOC],
			heap->heap_data[heap->heap_size - 1],
			len);

	}

	(heap->heap_size)--;
	rc = heap_min_heapify(heap, 0, compare);

	return rc;

}

/*
 * This function takes out an element from heap. Generally, heap removal is done
 * from the root of the heap.
 */
int heap_remove(heap_st *heap,
                void *data,
		size_t len,
                bool heap_type,
                common_data_compare_t compare)
{

	int rc = EOK;

	rc = heap_validate_input(heap, heap_type);
	if (rc != EOK)
	{
		return rc;
	}
	CHECK_RC_ASSERT((data == NULL), 0);
	
	if (heap_type == HEAP_TYPE_MIN)
	{
		rc = heap_remove_min(heap, data, len, compare);
	}
	else
	{
		rc = heap_remove_max(heap, data, len, compare);
	}

	return rc;

}

/*
 * This function prints integer node.
 */
void heap_print_int_node(void *data)
{

	if (data)
	{
		printf("%d\n", *(int*)data);
	}

}

/*
 * This function prints contents of heap.
 */
int heap_print_heap(heap_st *heap, heap_print_node_t printer)
{

	int rc = EOK;
	int i = 0;

	if ((heap) && (heap->heap_size == HEAP_MIN_SIZE))
	{

		rc = ENOENT;
		return rc;

	}

	for (i = 0; i < heap->heap_size; i++)
	{
		printer(heap->heap_data[i]);
	}

	return rc;

}

