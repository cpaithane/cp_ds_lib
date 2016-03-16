/*
 * Description : Find out longest valid substring from parentheses.
 */

#include "stack.h"

/*
 * This is main logic of solution. Idea is commented out in implementation.
 */
int stack_prob_12_main(stack_st *st, char *str)
{

	int i, len, rc, result;

	rc = EOK;
	result = 0;
	i = -1;
	len = strlen(str);

	/*
	 * Idea is to keep indexes of previos starting parentheses on stack.
	 */
	stack_push(st, &i, sizeof(int));

	for (i = 0; i < len; i++)
	{

		/*
		 * Push index if char is (
		 */
		if (str[i] == '(')
		{
			stack_push(st, &i, sizeof(int));
		}
		else if (str[i] == ')')
		{

			/*
			 * Pop from stack. Valid substring till this point is 
			 * i - peek of stack.
			 */
			stack_pop(st, &rc);
			if (!stack_is_stack_empty(st))
			{
				result = MAX(result, i - *(int*)stack_get_peek(st));
			}
			else
			{
				stack_push(st, &i, sizeof(int));
			}

		}

	}

	printf("Length of valid substring = %d\n", result);
	return rc;

}

/*
 * This is workhorse function for the problem.
 */
int stack_prob_12()
{

	int rc = EOK;
	char input[] = "()(()))))";
	stack_st *st;

	st = stack_alloc_stack(sizeof(int));
	rc = stack_prob_12_main(st, input);

	return rc;

}

