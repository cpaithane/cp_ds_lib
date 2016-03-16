#include "scll.h"

/*
 * This test case tests insertion at position 0.
 */
int scll_test_case1()
{

	int rc = EOK;
	scll_st *head = NULL;
	int i = 0;

	for (i = 0; i < 10; i++)
	{

		head = scll_insert_node_pos(head, &i, 0, sizeof(int));
		CHECK_RC_ASSERT((head == NULL), 0);
		scll_print_list(head);

	}

	rc = scll_remove_scll(head);
	CHECK_RC_ASSERT(rc, EOK);
	return rc;

}

/*
 * This function tests insertion at tail position.
 */
int scll_test_case2()
{

	int rc = EOK;
	scll_st *head = NULL;
	int i;

	for (i = 0; i < 10; i++)
	{

		head = scll_insert_node_pos(head, &i, i, sizeof(int));
		CHECK_RC_ASSERT((head == NULL), 0);
		scll_print_list(head);

	}

	rc = scll_remove_scll(head);
	CHECK_RC_ASSERT(rc, EOK);
	return rc;

}

/*
 * This function tests insertion at random position.
 */
int scll_test_case3()
{

	int rc = EOK;
	scll_st *head = NULL;
	int i;

	for (i = 0; i < 10; i++)
	{

		head = scll_insert_node_pos(head, &i, i, sizeof(int));
		CHECK_RC_ASSERT((head == NULL), 0);

	}

	head = scll_insert_node_pos(head, &i, 3, sizeof(int));

	i++;
	head = scll_insert_node_pos(head, &i, 5, sizeof(int));

	i++;
	head = scll_insert_node_pos(head, &i, 7, sizeof(int));
	CHECK_RC_ASSERT((head == NULL), 0);
	scll_print_list(head);
	rc = scll_remove_scll(head);
	CHECK_RC_ASSERT(rc, EOK);
	return rc;

}

/*
 * This function tests removal at tail position.
 */
int scll_test_case4()
{

	int rc = EOK;
	scll_st *head = NULL;
	int i;

	for (i = 0; i < 10; i++)
	{

		head = scll_insert_node_pos(head, &i, i, sizeof(int));
		CHECK_RC_ASSERT((head == NULL), 0);

	}

	scll_print_list(head);
	for (i = 0; i < 10; i++)
	{

		head = scll_remove_node_n(head, (10 - (i+1)));
		scll_print_list(head);

	}

	CHECK_RC_ASSERT(head, NULL);
	return rc;

}

/*
 * This function tests deletion at random position.
 */
int scll_test_case5()
{

	int rc = EOK;
	scll_st *head = NULL;
	int i;

	for (i = 0; i < 10; i++)
	{

		head = scll_insert_node_pos(head, &i, i, sizeof(int));
		CHECK_RC_ASSERT((head == NULL), 0);

	}

	head = scll_remove_node_n(head, 3);
	scll_print_list(head);
	head = scll_remove_node_n(head, 5);
	scll_print_list(head);
	head = scll_remove_node_n(head, 7);
	scll_print_list(head);

	for (i = 0; i < 3; i++)
	{

		head = scll_remove_node_n(head, 999);
		scll_print_list(head);

	}

	rc = scll_remove_scll(head);
	CHECK_RC_ASSERT(rc, EOK);

	return rc;

}

int scll_tc_execute()
{

	int rc = EOK;

	rc = scll_test_case1();
	rc = scll_test_case2();
	rc = scll_test_case3();
	rc = scll_test_case4();
	rc = scll_test_case5();

	return rc;

}

