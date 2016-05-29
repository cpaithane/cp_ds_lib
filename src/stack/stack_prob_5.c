/*
 * Find out next greater element for every member of the array. 
 * arr = {4, 5, 2, 25}
 * 
 * Element	NGE
 * 4		5
 * 5		25
 * 2		25
 * 25		-1
 */

#include "stack.h"

/*
 * Implementation of problem is defined here.
 */
int stack_prob_5()
{

	int rc = EOK;
	int arr[4] = {4, 5, 2, 25};
	int i, next, element;
	stack_st *st;
	void *node;

	st = stack_alloc_stack(sizeof(int));

	/*
	 * Lets push first element on stack.
	 */
	stack_push(st, &arr[0], sizeof(int));

	printf("Element --> NGE\n");
	for (i = 1; i < 4; i++)
	{

		next = arr[i];

		if (!stack_is_stack_empty(st))
		{

			/*
			 * Pop an element from stack.
			 */
			node = stack_pop(st, &rc);
			element = *(int*)node;

			/*
			 * If next is greater than element, next is nge.
			 */
			while (next > element)
			{

				printf ("%d --> %d\n", element, next);
				if (stack_is_stack_empty(st))
				{
					break;
				}

				node = stack_pop(st, &rc);
				element = *(int*)node;

			}

			if (element > next)
			{
				stack_push(st, &element, sizeof(int));
			}

		}

		stack_push(st, &next, sizeof(int));

	}

	/*
	 * Print all the remaining elements from stack with NGE as -1
	 */
	while (!stack_is_stack_empty(st))
	{

		node = stack_pop(st, &rc);
		element = *(int*)node;
		printf ("%d --> -1\n", element);

	}
	printf("\n");
	stack_dealloc_stack(st);
	return rc;

}

