/*
 * Description : Reverse a singly linked list by :
 * 1. Iterative method
 * 2. Recursive method
 */

#include "sll.h"

/*
 * This function has implementation for iterative way of reversal.
 */
sll_st* sll_prob_1_rev_iter(sll_st *head)
{

	sll_st *cur, *prev, *next;

	cur = head;
	prev = NULL;

	while (cur != NULL)
	{

		next = cur->sll_next;
		cur->sll_next = prev;
		prev = cur;
		cur = next;

	}

	/*
	 * As whole linked list is traversed, change head pointer to prev.
	 */
	head = prev;
	return head;

}

/*
 * This function has implementation for recursive reversal of linked list.
 */
sll_st* sll_prob_1_rev_recur(sll_st *cur, sll_st *prev)
{

	if (cur->sll_next == NULL)
	{

		cur->sll_next = prev;
		return cur;

	}

	sll_st *next = cur->sll_next;
	cur->sll_next = prev;
	sll_prob_1_rev_recur(next, cur);

}

/*
 * This function calls an iterative reverse on a linked list and reverses the same 
 * using recursive method.
 */
int ssl_prob_1()
{

	int i, rc = EOK;
	sll_st *head = NULL;

	for (i = 0; i < 10; i++)
	{
		head = sll_insert_node_pos(head, &i, 0, sizeof(int));
	}

	printf("Original linked list = \n");
	sll_print_sll(head);

	printf("Reversed using iterative method = \n");
	head = sll_prob_1_rev_iter(head);
	sll_print_sll(head);

	printf("Reversed using recursive method = \n");
	head = sll_prob_1_rev_recur(head, NULL);
	sll_print_sll(head);

	rc = sll_remove_sll(head);
	return rc;

}

