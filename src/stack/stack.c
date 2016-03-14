#include "stack.h"

/*
 * This function allocates new stack instance. len indicates size of individual node in stack.
 */
stack_st * stack_alloc_stack(size_t len)
{

	stack_st *st = NULL;
	void *node = NULL;	
	int i;

	st = stack_get_new_node(sizeof(stack_st));

	for (i = 0; i < STACK_MAX_ITEMS; i++)
	{

		node = stack_get_new_node(len);
		st->st_node[i] = node;

	}

	stack_initialize(st);
	return st;

}

/*
 * This function allocates new node of the stack
 */
void *stack_get_new_node(size_t len)
{

	void *node = NULL;
	node = malloc(len);
	memset(node, 0, len);
	return node;

}

/*
 * This function deallocates stack instance
 */
int stack_dealloc_stack(stack_st *st)
{

	int rc = EOK;
	int i;

	/*
	 * If top is -1, then free the stack, else return.
	 */
	if (st->st_top == STACK_MIN_ITEMS)
	{

		for (i = 0; i < STACK_MAX_ITEMS; i++)
		{

			free(st->st_node[i]);
			st->st_node[i] = NULL;

		}

		free(st);

	}
	else
	{
		rc = ENOTEMPTY;
	}

	return rc;

}

/*
 * This function initializes whole stack. Resets st_top to -1.
 */
void stack_initialize(stack_st *st)
{

	st->st_top = STACK_MIN_ITEMS;

}

/*
 * This function returns element stored at top of stack.
 */
void *stack_get_peek(stack_st *st)
{

	void *data = NULL;
	if (!stack_is_stack_empty(st))
	{
		data = st->st_node[st->st_top];
	}

	return data;

}

/*
 * This function checks whether the new node is pushable on stack or not.
 */
int stack_is_stack_full(stack_st *st)
{

	int rc = EOK;

	if (st->st_top == (STACK_MAX_ITEMS - 1))
	{
		rc = EOVERFLOW;
	}

	return rc;

}

/*
 * This function determines whether the stack has sufficient elements to pop.
 */
int stack_is_stack_empty(stack_st *st)
{

	int is_stack_empty = 0;

	if (st->st_top == STACK_MIN_ITEMS)
	{
		is_stack_empty = 1;
	}

	return is_stack_empty;

}

/*
 * This function pushes data on top of stack. Returns return code of the push 
 * operation.
 */
int stack_push(stack_st *st, void *data, size_t len)
{

	int rc = EOK;
	void *node = NULL;

	rc = stack_is_stack_full(st);
	CHECK_RC(rc, EOK);

	(st->st_top)++;
	memcpy(st->st_node[st->st_top], data, len);

	return rc;

}

/*
 * This function pops node stored at the location of top of stack. Status is returned
 * as return code of the operation. 
 */
void *stack_pop(stack_st *st, int *status)
{

	void *node = NULL;
	int rc = EOK;

	rc = stack_is_stack_empty(st);
	if (rc != EOK)
	{

		*status = ENOENT;
		return node;

	}

	*status = EOK;
	node = st->st_node[st->st_top];
	(st->st_top)--;

	return node;

}

/*
 * This function prints nodes of stack starting from top. Considering integer data stored in stack.
 */
int stack_list(stack_st *st)
{

	int rc = EOK;
	int i;
	int *node;

	rc = stack_is_stack_empty(st);
	CHECK_BOOL_RET_RC(rc, EOK, ENOENT);

	for (i = st->st_top; i >= 0; i--)
	{

		node = (int *)st->st_node[i];
		printf("%d\n", *node);

	}

	return rc;

}

/*
 * This function compares integer data and returns 
 * 0 if both are equal
 * 1 if data1 > data2
 * -1 if data1 < data2
 */
int stack_int_data_compare(const void *data1, const void *data2)
{

	int ret_val = IDENTICAL;
	int val1 = *((int *)data1);
	int val2 = *((int *)data2);

	if (val1 < val2)
	{
		ret_val = FIRST_LESS;
	}
	else if (val1 > val2)
	{
		ret_val = FIRST_GREATER;
	}
	
	return ret_val;

}

