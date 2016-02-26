#include "sll.h"

/*
 * This test case tests inserts/deletes at head
 */
int sll_test_case1()
{

	int rc = EOK;
	int i = 0;
	sll_st *sll = NULL;

	sll = sll_insert_node_pos(sll, &i, 0, sizeof(int));
	CHECK_RC_ASSERT((sll == NULL), 0);
	sll_print_sll(sll);

	rc = sll_remove_sll(sll);
	CHECK_RC_ASSERT(rc, EOK);
	return rc;

}

/*
 * This test case inserts 5 nodes inside a linked list. After that, it tries to input
 * position as 10. The node should be placed at tail of linked list.
 */
int sll_test_case2()
{

	int rc = EOK;
	int i = 0;
	sll_st *sll = NULL;

	for (i = 0; i < 5; i++)
	{

		sll = sll_insert_node_pos(sll, &i, i, sizeof(int));
		CHECK_RC_ASSERT((sll == NULL), 0);

	}

	sll_print_sll(sll);

	rc = sll_remove_sll(sll);
	CHECK_RC_ASSERT(rc, EOK);

	return rc;

}

/*
 * This test case tests insertion at random places.
 */
int sll_test_case3()
{

	int rc = EOK;
	int i = 0;
	sll_st *sll = NULL;

	/*
	 * Insert 5 items into linked list one bye one and print it.
	 */
	for (i = 0; i < 5; i++)
	{

		sll = sll_insert_node_pos(sll, &i, i, sizeof(int));
		CHECK_RC_ASSERT((sll == NULL), 0);

	}

	sll_print_sll(sll);

	/*
	 * Insert at position 0.
	 */
 
	for (i = 5; i < 10; i++)
	{

		sll = sll_insert_node_pos(sll, &i, 0, sizeof(int));
		CHECK_RC_ASSERT((sll == NULL), 0);

	}

	sll_print_sll(sll);

	/*
	 * Insert at position 3 in linked list of 10 nodes
	 */
	sll = sll_insert_node_pos(sll, &i, 3, sizeof(int));
	CHECK_RC_ASSERT((sll == NULL), 0);
	sll_print_sll(sll);

	/*
	 * Insert at tail node of linked list of 11 nodes
	 */
	i++;
	sll = sll_insert_node_pos(sll, &i, 12, sizeof(int));
	CHECK_RC_ASSERT((sll == NULL), 0);
	sll_print_sll(sll);

	i++;
	sll = sll_insert_node_pos(sll, &i, 13, sizeof(int));
	CHECK_RC_ASSERT((sll == NULL), 0);
	sll_print_sll(sll);

	/*
	 * Insert at a position which is very high
	 * It should insert at tail node of linked list
	 */
	i = 999;
	sll = sll_insert_node_pos(sll, &i, 999, sizeof(int));
	CHECK_RC_ASSERT((sll == NULL), 0);
	sll_print_sll(sll);

	rc = sll_remove_sll(sll);
	CHECK_RC_ASSERT(rc, EOK);

	return rc;

}

/*
 * This test case tests deletion at random places.
 */
int sll_test_case4()
{

	int rc = EOK;
	int i = 0;
	sll_st *sll = NULL;

	/*
	 * Insert 10 items into linked list one bye one and print it.
	 */
	for (i = 0; i < 10; i++)
	{

		sll = sll_insert_node_pos(sll, &i, i, sizeof(int));
		CHECK_RC_ASSERT((sll == NULL), 0);

	}

	sll_print_sll(sll);

	/*
	 * Remove at position 0 and print
	 */
	for (i = 0; i < 10; i++)
	{
	
		sll = sll_remove_node_pos(sll, 0);
		sll_print_sll(sll);

	}

	rc = sll_remove_sll(sll);
	CHECK_RC_ASSERT(rc, EOK);
	return rc;

}

/*
 * This test case tests deletion at random places.
 */
int sll_test_case5()
{

	int rc = EOK;
	int i = 0;
	sll_st *sll = NULL;

	/*
	 * Insert 10 items into linked list one bye one and print it.
	 */
	for (i = 0; i < 10; i++)
	{

		sll = sll_insert_node_pos(sll, &i, i, sizeof(int));
		CHECK_RC_ASSERT((sll == NULL), 0);

	}

	sll_print_sll(sll);

	/*
	 * Remove at 3, 5, and 7th position
	 */
	sll = sll_remove_node_pos(sll, 3);
	sll_print_sll(sll);

	sll = sll_remove_node_pos(sll, 5);
	sll_print_sll(sll);

	sll = sll_remove_node_pos(sll, 7);
	sll_print_sll(sll);

	/*
	 * Remove at last node position and print
	 * Note : 3 nodes are deleted.
	 */
	for (i = 0; i < 7; i++)
	{
	
		sll = sll_remove_node_pos(sll, (7 - (i+1)));
		sll_print_sll(sll);

	}

	rc = sll_remove_sll(sll);
	CHECK_RC_ASSERT(rc, EOK);

}

/*
 * This test case tests deletion at a non-existent place
 */
int sll_test_case6()
{

	int rc = EOK;
	int i = 0;
	sll_st *sll = NULL;

	/*
	 * Insert 10 items into linked list one bye one and print it.
	 */
	for (i = 0; i < 10; i++)
	{

		sll = sll_insert_node_pos(sll, &i, i, sizeof(int));
		CHECK_RC_ASSERT((sll == NULL), 0);

	}

	sll_print_sll(sll);

	for (i = 0; i < 10; i++)
	{
	
		sll = sll_remove_node_pos(sll, 99);
		sll_print_sll(sll);

	}

	rc = sll_remove_sll(sll);
	CHECK_RC_ASSERT(rc, EOK);
}

int main()
{

	int rc = EOK;

	rc = sll_test_case1();
	rc = sll_test_case2();
	rc = sll_test_case3();
	rc = sll_test_case4();
	rc = sll_test_case5();
	rc = sll_test_case6(); 

	return rc;

}

