/*
 * Description : Design a stack which supports get_min() in O(1)
 */

#include "stack.h"

/*
 * This function pops from two stacks maintained by push. 
 */
void* stack_prob_9_pop(stack_st *st_main, stack_st *st_temp, int *rc)
{

	stack_pop(st_temp, rc);
	return (stack_pop(st_main, rc));

}

/*
 * This function maintains two stacks : 
 * st_main : For pushing elements
 * st_temp : For storing minimum elements
 */
int stack_prob_9_push(stack_st *st_main, stack_st *st_temp, int i)
{

	int min, rc = EOK;
	int *node;

	node = (int*)stack_get_peek(st_temp);

	if (node == NULL)
	{
		min = i;
	}
	else
	{
		min = *node;
	}

	if (i < min)
	{
		min = i;
	}

	stack_push(st_main, &i, sizeof(int));
	stack_push(st_temp, &min, sizeof(int));
	
	return rc;

}

/*
 * This function is workhorse for the problem described above.
 */
int stack_prob_9()
{

	int i, *node, rc = EOK;

	stack_st *st_main = NULL;
	stack_st *st_temp = NULL;

	st_main = stack_alloc_stack(sizeof(int));
	st_temp = stack_alloc_stack(sizeof(int));

	for (i = 0; i < 10; i++)
	{

		stack_prob_9_push(st_main, st_temp, i);
		printf("Min element after push : %d\n", 
		       *(int*)stack_get_peek(st_temp));

	}

	printf("Main stack = \n");
	stack_list(st_main);
	printf("Temp stack = \n");
	stack_list(st_temp);

	for (i = 0; i < 10; i++)
	{

		stack_prob_9_pop(st_main, st_temp, &rc);
		node = (int*)stack_get_peek(st_temp);
		if (node)
		{
			printf("Min element after pop : %d\n", *node);
		}

	}

	stack_make_stack_empty(st_main);
	stack_make_stack_empty(st_temp);
	return rc;

}

