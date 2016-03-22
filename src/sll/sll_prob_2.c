/*
 * Description : Merge sort on singly linked list.
 */

#include "sll.h"

typedef int (*common_data_compare_t)(const void *, const void *);

/*
 * Merge the linked lists in sorted order.
 */
sll_st *sll_sorted_merge(sll_st *sll1, 
			 sll_st *sll2, 
			 common_data_compare_t compare)
{

	sll_st *head = NULL;

	/*
	 * Either of the linked lists is NULL, return another.
	 */
	if (sll1 == NULL)
	{

		head = sll2;
		return head;

	}

	if (sll2 == NULL)
	{

		head = sll1;
		return head;

	}

	if (compare(sll1->sll_data, sll2->sll_data) == FIRST_LESS)
	{

		head = sll1;
		head->sll_next = sll_sorted_merge(sll1->sll_next, sll2, 
				 compare);

	}
	else
	{

		head = sll2;
		head->sll_next = sll_sorted_merge(sll1, sll2->sll_next, 
				 compare);

	}

	return head;

}

/*
 * This function splits the linked list from middle.
 */
void sll_split(sll_st *head, sll_st **sll1, sll_st **sll2)
{

	sll_st *slow, *fast;

	/*
	 * Nothing to split if there is no or one node in the list.
	 */
	if ((head == NULL) || (head->sll_next == NULL))
	{

		*sll1 = head;
		*sll2 = NULL;

	}


	/*
	 * Use two pointers for traversal of linked list, one should be slow and 
	 * another should be of double speed of slow. When fast reaches end of the
	 * linked list, slow is pointing at middle of linked list.
	 */
	slow = head;
	fast = head->sll_next;
	while (fast != NULL)
	{

		fast = fast->sll_next;

		if (fast)
		{
	
			slow = slow->sll_next;
			fast = fast->sll_next;

		}

	}

	/*
	 * Form two lists now. sll1 should start from head and sll2 should start 
	 * from middle of list.
	 * Also, break the list into two parts because divide and conquer will be 
	 * used for merge sort.
	 */
	*sll1 = head;
	*sll2 = slow->sll_next;
	slow->sll_next = NULL;

}

/*
 * This function sorts the linked list in recursive manner.
 */
sll_st* sll_merge_sort(sll_st **head_ref, common_data_compare_t compare)
{

	sll_st *head = *head_ref;
	sll_st *sll1, *sll2;

	/*
	 * If there is nothing to sort, return linked list as is.
	 */
	if ((head == NULL) || (head->sll_next == NULL))
	{
		return head;
	}

	sll_split(head, &sll1, &sll2);
	sll_merge_sort(&sll1, compare);
	sll_merge_sort(&sll2, compare);

	*head_ref = sll_sorted_merge(sll1, sll2, compare);
	return *head_ref;

}

/*
 * Workhorse function for merge sort.
 */
int sll_prob_2()
{

	int rc = EOK;
	int data[15] = {1000, 500, 250, 750, 775, 800, 825, 850, 900, 925, 950, 125,
			150, 175, 200};
	int i = 0;
	sll_st *sll = NULL;

	/*
	 * Insert 15 items into linked list one bye one and print it.
	 */
	for (i = 0; i < 15; i++)
	{

		sll = sll_insert_node_pos(sll, &data[i], i, sizeof(int));
		CHECK_RC_ASSERT((sll == NULL), 0);

	}

	printf("Singly linked list before sorting = \n");
	sll_print_sll(sll);

	sll = sll_merge_sort(&sll, common_int_data_compare);

	printf("Singly linked list after sorting = \n");
	sll_print_sll(sll);

	rc = sll_remove_sll(sll);
	return rc;

}

