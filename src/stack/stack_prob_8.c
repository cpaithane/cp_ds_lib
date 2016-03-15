/*
 * Stock span problem : 
 * Stock span for i'th day is defined as maximum no. of consecutive days in the past
 * stock was greater than current day. 
 */

#include "stack.h"

/*
 * This is workhorse function for calculation of spans.
 */
int stack_prob_8()
{

	int i, rc = EOK;
	stack_st *st = NULL;
	int days[7] = {100, 80, 60, 70, 60, 75, 85};
	int span[7] = {0, 0, 0, 0, 0, 0, 0};
	void *node;

	st = stack_alloc_stack(sizeof(int));

	/*
	 * Push first value and initialize its span as 1.
	 */
	i = 0;
	span[0] = 1;
	rc = stack_push(st, &i, sizeof(int));
	CHECK_RC_ASSERT(rc, EOK);

	for (i = 1; i < 7; i++)
	{

		while (!stack_is_stack_empty(st) && 
			(days[*(int*)(stack_get_peek(st))] <= days[i]))
		{
			stack_pop(st, &rc);
		}

		if (stack_is_stack_empty(st))
		{
			span[i] = i + 1;
		}
		else
		{
			span[i] = i - *(int*)stack_get_peek(st);
		}

		stack_push(st, &i, sizeof(int));

	}

	printf("Span of stock = ");
	for (i = 0; i < 7; i++)
	{
		printf("%d ", span[i]);
	}

	printf("\n");
	rc = stack_dealloc_stack(st);
	return rc;

}

