/*
 * Description : Sort a stack without using looping constructs.
 */

#include "stack.h"

/*
 * This function does following in order to insert element at bottom :
 * 1. Pops stack till it is empty.
 * 2. If stack is empty or data is in proper order push the element on stack.
 * 3. Push the elements hold on the function stack on stack one by one.
 */
void stack_prob_7_sorted_insert(stack_st *st, 
				void *node, 
				size_t len, 
				stack_data_compare_t compare)
{

	int rc = EOK;
	void *temp, *temp2;

	if (stack_is_stack_empty(st) || 
	   (compare(node, stack_get_peek(st)) == FIRST_GREATER))
	{

		temp = malloc(len);
		memcpy(temp, node, len);
		stack_push(st, temp, len);
		free(temp);

	}
	else
	{

		temp = stack_pop(st, &rc);
		temp2 = malloc(len);
		memcpy(temp2, temp, len);
		stack_prob_7_sorted_insert(st, node, len, compare);
		stack_push(st, temp2, len);
		free(temp2);

	}

}

/*
 * This function does following : 
 * Pop till stack finishes, insert all the item at bottom of stack.
 */
void stack_prob_7_sort(stack_st *st, size_t len, stack_data_compare_t compare)
{

	int rc = EOK;
	void *node = NULL;

	if (stack_is_stack_empty(st))
	{
		return;
	}

	node = stack_pop(st, &rc);
	stack_prob_7_sort(st, len, compare);
	stack_prob_7_sorted_insert(st, node, len, compare);
	
}

/*
 * This is a workhorse function for the problem mentioned in description section.
 */
int stack_prob_7()
{

	int i;
	int rc = EOK;
	stack_st *st = NULL;
	int arr[5] = {100, 10, 200, 20, 300};

        st = stack_alloc_stack(sizeof(int));

	/*
	 * Push 5 itms on stack 
	 */
	for (i = 0; i < 5; i++)
	{

		rc = stack_push(st, &arr[i], sizeof(int));
		CHECK_RC_ASSERT(rc, EOK);

	}

	printf("Stack before sorting = \n");
	stack_list(st);

	/*
	 * Now, reverse the stack.
	 */
	stack_prob_7_sort(st, sizeof(int), stack_int_data_compare);

	printf("Stack after sorting = \n");
	stack_list(st);

	for (i = 0; i < 5; i++)
	{

		stack_pop(st, &rc);
		CHECK_RC_ASSERT(rc, EOK);

	}

	rc = stack_dealloc_stack(st);
	return rc;

}

