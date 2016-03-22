/*
 * Description : Check if given array can represent preorder traversal of BST.
 */

#include "stack.h"

/*
 * Logic is similar to find next greater element.
 */
int stack_prob_13_main(stack_st *st, int *arr, int count)
{

	int rc = EOK;
	int i;

	/*
	 * Assume a root as minimum of integer.
	 */
	int root = INT_MIN;
	
	for (i = 0; i < count; i++)
	{

		/*
		 * If an element in right subtree with smaller value than root, then
		 * it is not preorder.
		 */
		if (arr[i] < root)
		{

			rc = 1;
			break;

		}

		/*
		 * Items from array are found in right subtree. Keep popping and 
		 * find new root.
		 */
		while ((!stack_is_stack_empty(st)) && 
		       (*(int *)stack_get_peek(st) < arr[i]))
		{
			root = *(int *)stack_pop(st, &rc);
		}

		stack_push(st, &(arr[i]), sizeof(int));

	}

	return rc;

}

/*
 * This is a workhorse function for the problem stated in description.
 */
int stack_prob_13()
{

	int rc = EOK;
	stack_st *st;
	int arr[5] = {40, 30, 35, 80, 100};

	st = stack_alloc_stack(sizeof(int));

	rc = stack_prob_13_main(st, arr, 5);
	if (rc)
	{
		printf("Array does not represent preorder traversal\n");
	}
	else
	{
		printf("Array represents preorder traversal\n");
	}

	stack_dealloc_stack(st);

	return rc;

}

