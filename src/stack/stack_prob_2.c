/*
 * Evaluation of postfix expression.
 */

#include "stack.h"

/*
 * This function evaluates from inputs and returns result
 */
int stack_evaluate_values(int val1, int val2, char operator)
{

	int result = 0;

	switch(operator)
	{

		case '+':
			result = val1 + val2;
			break;

		case '-':
			result = val1 - val2;
			break;

		case '*':
			result = val1 * val2;
			break;

		case '/':
			result = val1 / val2;
			break;

		case '^':
			result = val1 ^ val2;
			break;

		default:
			CHECK_RC_ASSERT(1, 0);
			break;

	}

	return result;

}

/*
 * This function inputs postfix expression as a string and prints its evaluation.
 * Assumptions : 
 * 	1. Supported operators : +, -, *, /, ^.
 *	2. Single digit decimals are allowed.
 */
int stack_prob_2(char *input)
{

	int rc, result, i, val1;
	char ch, operator;
	int *node1, *node2;
	stack_st *st;

	CHECK_RC_ASSERT((input == NULL), 0);

	st = stack_alloc_stack(sizeof(int));

	i = 0;
	while ((ch = input[i++]) != '\0')
	{

		/*
		 * If character is between 0 to 9, then push it on stack.
		 * Else, pop twice from stack, evaluate and push the result back to 
		 * stack.
		 */
		if (isdigit(ch))
		{

			val1 = ch - '0';
			stack_push(st, &val1, sizeof(int));

		}
		else
		{

			operator = ch;
			node2 = (int *)stack_pop(st, &rc);
			CHECK_RC_ASSERT(rc, EOK);
		
			node1 = (int *)stack_pop(st, &rc);
			CHECK_RC_ASSERT(rc, EOK);
			
			result = stack_evaluate_values(*node1, *node2, operator);
			stack_push(st, &result, sizeof(int));

		}

	}

	result = *(int *)stack_pop(st, &rc);
	rc = stack_dealloc_stack(st);
	printf("Evaluation of postfix expression = %d\n", result);
	return rc;

}

