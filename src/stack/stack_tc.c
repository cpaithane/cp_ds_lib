#include "stack.h"

/*
 * This test case pushes STACK_MAX_ITEMS + 1 integers on stack.
 */
int stack_test_case1()
{

	int rc = EOK;
	int i;
	stack_st *st = NULL;
	void *node = NULL;

	st = stack_alloc_stack(sizeof(int));

	/*
	 * Push till stack is full and check that further push on stack is not 
	 * allowed.
	 */
	for (i = 0; i < STACK_MAX_ITEMS; i++)
	{

		rc = stack_push(st, &i, sizeof(int));
		CHECK_RC_ASSERT(rc, EOK);

	}

	rc = stack_push(st, &i, sizeof(int));
	CHECK_RC_ASSERT(rc, EOVERFLOW);

	rc = stack_list(st);
	CHECK_RC_ASSERT(rc, EOK);

	/*
	 * Pop all the stack items and check that further pop is not allowed.
	 */
	for (i = 0; i < STACK_MAX_ITEMS; i++)
	{

		stack_pop(st, &rc);
		CHECK_RC_ASSERT(rc, EOK);

	}

	CHECK_RC_ASSERT(st->st_top, STACK_MIN_ITEMS);

	stack_pop(st, &rc);
	CHECK_RC_ASSERT(rc, ENOENT);

	rc = stack_list(st);
	CHECK_RC_ASSERT(rc, ENOENT);

	rc = stack_dealloc_stack(st);
	CHECK_RC_ASSERT(rc, EOK);
	return rc;

}

int stack_tc_execute()
{

	int rc = EOK;
	char str[] = "GeeksForGeeks";

	rc = stack_test_case1();
	rc = stack_prob_1(str);

	char postfix_str[] = "231*+9-";
	rc = stack_prob_2(postfix_str);

	char infix_str[] = "a+b*(c^d-e)^(f+g*h)-i";
	rc = stack_prob_3(infix_str);

	char parantheses1[] = ")()";
	rc = stack_prob_4(parantheses1);

	char parantheses2[] = "[{()}]";
	rc = stack_prob_4(parantheses2);

	rc = stack_prob_5();
	rc = stack_prob_6();
	rc = stack_prob_7();
	rc = stack_prob_8();
	rc = stack_prob_9();
	rc = stack_prob_10();
	rc = stack_prob_11();
	rc = stack_prob_12();
	rc = stack_prob_13();
	return rc;

}

