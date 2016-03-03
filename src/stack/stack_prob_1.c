/*
 * Description of problem :
 * Given a string, reverse it using stack data structure.
 * Input : "GeeksForGeeks"
 * Output : "skeeGrofFskeeG"
 */

#include "stack.h"

/*
 * This function pops every character from stack and places it in input string 
 * itself.
 */
int stack_prob_1_gen_rev(stack_st *st, char *input)
{

	int rc;
	char *data;
	int i = 0;

	/*
	 * Pop till stack is empty and insert inside input string.
	 */
	while (stack_is_stack_empty(st) != 1)
	{

		data = (char *)stack_pop(st, &rc);
		if (rc == EOK)
		{
			input[i++] = *data;
		}
		else
		{
			break;
		}

	}

	CHECK_RC_ASSERT(rc, EOK);

	/*
	 * Print input string now, which is reversed.
	 */
	printf("Reversed string = %s\n", input);

	return rc;

}

/*
 * This function places every character on stack.
 */
void stack_prob_1_fill(stack_st *st, char *input)
{

	int len, i, rc;

	len = strlen(input);
	CHECK_RC_ASSERT((input == NULL), 0);

	/*
	 * Push every character on stack.
	 */
	for (i = 0; i < len; i++)
	{

		rc = stack_push(st, &input[i], sizeof(char));
		CHECK_RC_ASSERT(rc, EOK);

	}

	printf("Input string = %s\n", input);

}

/*
 * This is workhorse function to solve problem 1 described above.
 */
int stack_prob_1(char *input)
{

	int rc; 
	stack_st *st;

	st = stack_alloc_stack(sizeof(char));
	stack_prob_1_fill(st, input);

	rc = stack_prob_1_gen_rev(st, input);
	CHECK_RC_ASSERT(rc, EOK);

	rc = stack_dealloc_stack(st);
	CHECK_RC_ASSERT(rc, EOK);

	return rc;

}

