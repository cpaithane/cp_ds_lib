#include "dll.h"

/*
 * This test case tests insertion at head node.
 */
int dll_test_case1()
{

	int rc = EOK;
	int i;
	dll_st *head = NULL;

	for (i = 0; i < 10; i++)
	{

		head = dll_insert_node_pos(head, &i, sizeof(int), 0);
		CHECK_RC_ASSERT((head == NULL), 0);
		dll_print_list(head);

	}
	
	dll_print_list_rev(head);

	dll_remove_dll(head);

	return rc;

}

/*
 * This test case tests insertion at tail node.
 */
int dll_test_case2()
{

	int rc = EOK;
	int i;
	dll_st *head = NULL;

	for (i = 0; i < 10; i++)
	{

		head = dll_insert_node_pos(head, &i, sizeof(int), i);
		CHECK_RC_ASSERT((head == NULL), 0);
		dll_print_list(head);

	}
	
	dll_print_list_rev(head);

	dll_remove_dll(head);

	return rc;

}

/*
 * This test case tests insertion at non-head/tail node.
 */
int dll_test_case3()
{

	int rc = EOK;
	int i;
	dll_st *head = NULL;

	for (i = 0; i < 10; i++)
	{

		head = dll_insert_node_pos(head, &i, sizeof(int), i);
		CHECK_RC_ASSERT((head == NULL), 0);
		dll_print_list(head);

	}
	
	dll_print_list_rev(head);

	head = dll_insert_node_pos(head, &i, sizeof(int), 3);
	dll_print_list(head);

	i++;
	head = dll_insert_node_pos(head, &i, sizeof(int), 5);
	dll_print_list(head);

	i++;
	head = dll_insert_node_pos(head, &i, sizeof(int), 5);
	dll_print_list(head);

	dll_print_list_rev(head);

	for (i = 0; i < 5; i++)
	{

		head = dll_insert_node_pos(head, &i, sizeof(int), 999);
		dll_print_list(head);

	}
	dll_print_list_rev(head);
	dll_remove_dll(head);

	return rc;

}

/*
 * This test case tests deletion at tail node.
 */
int dll_test_case4()
{

	int rc = EOK;
	int i;
	dll_st *head = NULL;

	for (i = 0; i < 10; i++)
	{

		head = dll_insert_node_pos(head, &i, sizeof(int), i);
		CHECK_RC_ASSERT((head == NULL), 0);
		dll_print_list(head);

	}

	head = dll_remove_node_pos(head, 999);
	dll_print_list(head);

	head = dll_remove_node_pos(head, 999);
	dll_print_list(head);

	head = dll_remove_node_pos(head, 999);
	dll_print_list(head);

	for (i = 0; i < 7; i++)
	{

		dll_print_list(head);
		head = dll_remove_node_pos(head, (10 - (i+1)));

	}

	return rc;

}

int dll_tc_execute()
{

	int rc = EOK;

	rc = dll_test_case1();
	rc = dll_test_case2();
	rc = dll_test_case3();
	rc = dll_test_case4();

	/*
	 * Work and fix problem 1 properly. There is elusive memory leak.
	rc = dll_prob_1();
	 */
	return rc;

}

