/*
 * Description : Design a stack with operations on middle element with O(1) 
 * complexity.
 */

#include "../dll/dll.h"

typedef struct stack_dll
{

	dll_st *head;
	dll_st *middle;
	int count;

}stack_dll_st;

/*
 * This function allocates and resets the stack_dll_st data structure.
 */
stack_dll_st* stack_dll_allocate()
{

	stack_dll_st* st = (stack_dll_st*)malloc(sizeof(stack_dll_st));
	memset(st, 0, sizeof(stack_dll_st));

	st->head = NULL;
	st->middle = NULL;
	st->count = 0;

	return st;

}

/*
 * This function deallocates doubly linked list etc.
 */
void stack_dll_deallocate(stack_dll_st *st)
{

	if (st->count != 0)
	{
		dll_remove_dll(st->head);
	}

	free(st);

}

/*
 * This function prints stack alongwith middle element.
 */
void stack_prob_11_print_mid(stack_dll_st *st)
{

	printf("Stack = \n");
	dll_print_list(st->head);

	if (st->middle)
	{
		printf("Middle element = %d\n\n", *(int*)(st->middle->dll_data));
	}

}

/*
 * This function inserts data into doubly linked list at head position. Also, it 
 * maintains middle pointer of the stack.
 */
int stack_prob_11_push(stack_dll_st *st, void *data, size_t size)
{

	int rc = EOK;

	CHECK_RC_ASSERT((st == NULL), 0);
	st->head = dll_insert_node_pos(st->head, data, size, 0);
	CHECK_RC_ASSERT((st->head == NULL), 0);
	st->count += 1;
	
	if (st->count == 1)
	{
		st->middle = st->head;
	}
	else if (((st->count) % 2 == 1))
	{
		st->middle = st->middle->dll_prev;
	}

	return rc;

}

/*
 * This function pops an element from node position 0 and maintains middle pointer.
 */
void* stack_prob_11_pop(stack_dll_st *st, size_t size)
{

	CHECK_RC_ASSERT((st == NULL), 0);

	/*
	 * Free this memory after usage from caller.
	 */
	void *node = malloc(size);

	memcpy(node, st->head->dll_data, size);
	st->head = dll_remove_node_pos(st->head, 0);
	st->count -= 1;

	if (st->count == 0)
	{
		st->middle = NULL;
	}
	else if ((st->count % 2) == 0)
	{
		st->middle = st->middle->dll_next;
	}

	return node;

}

/*
 * This is a workhorse function for problem described above. We should not use array
 * for this problem because insertion/deletions in the array are of complexity O(1).
 * So, I prefer using linked list to solve the problem.
 */
int stack_prob_11()
{

	int i, rc = EOK;
	stack_dll_st *st;
	void *node;

	st = stack_dll_allocate();

	for (i = 0; i < 10; i++)
	{

		rc = stack_prob_11_push(st, &i, sizeof(int));
		CHECK_RC_ASSERT(rc, EOK);
		stack_prob_11_print_mid(st);

	}

	for (i = 0; i < 10; i++)
	{

		node = stack_prob_11_pop(st, sizeof(int));
		CHECK_RC_ASSERT(rc, EOK);
		stack_prob_11_print_mid(st);
		printf("Popped element = %d\n", *(int*)node);
		free(node);

	}

	stack_dll_deallocate(st);
	return rc;

}

