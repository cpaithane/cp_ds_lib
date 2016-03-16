#include "bst.h"

extern bst_st *bst_root;
/*
 * This test case tests insertion with 7 nodes in rightmost side.
 */
int bst_test_case1()
{

	int rc = EOK;
	int i;
	int data[7] = {100, 10, 200, 20, 400, 30, 600};

	for (i = 0; i < 7; i++)
	{

		bst_insert(bst_root, &i, sizeof(int), bst_int_data_compare);
		CHECK_RC_ASSERT((bst_root == NULL), 0);

	}

	bst_inorder_traversal(bst_root, bst_int_node_printer);
	bst_destroy_tree(bst_root);
	bst_root = NULL;
	CHECK_RC_ASSERT(bst_root, NULL);

	return rc;

}

/*
 * This test case tests insertion with 7 nodes in leftmost side.
 */
int bst_test_case2()
{

	int rc = EOK;
	int i;
	int data[7] = {100, 10, 200, 20, 400, 30, 600};

	for (i = 10; i > 0; i--)
	{

		bst_insert(bst_root, &i, sizeof(int), bst_int_data_compare);
		CHECK_RC_ASSERT((bst_root == NULL), 0);

	}

	bst_inorder_traversal(bst_root, bst_int_node_printer);
	bst_destroy_tree(bst_root);
	bst_root = NULL;
	CHECK_RC_ASSERT(bst_root, NULL);

	return rc;

}

/*
 * This function inserts random data inside BST.
 */
int bst_test_case3()
{

	int rc = EOK;
	int i;
	int data[7] = {100, 10, 200, 20, 400, 30, 600};

	for (i = 0; i < 7; i++)
	{

		bst_insert(bst_root, &(data[i]), sizeof(int), bst_int_data_compare);
		CHECK_RC_ASSERT((bst_root == NULL), 0);

	}

	bst_inorder_traversal(bst_root, bst_int_node_printer);
	bst_destroy_tree(bst_root);
	bst_root = NULL;
	CHECK_RC_ASSERT(bst_root, NULL);

	return rc;

}

/*
 * This test case tests deletion of leaf node.
 */
int bst_test_case4()
{

	int rc = EOK;
	int i;

	for (i = 0; i < 7; i++)
	{

		bst_insert(bst_root, &i, sizeof(int), bst_int_data_compare);
		CHECK_RC_ASSERT((bst_root == NULL), 0);

	}

	bst_inorder_traversal(bst_root, bst_int_node_printer);

	for (i = 6; i >= 0; i--)
	{

		bst_delete_node(bst_root, &i, sizeof(int), bst_int_data_compare);
		printf("After deleting %d\n", i);
		bst_inorder_traversal(bst_root, bst_int_node_printer);

	}

	bst_root = NULL;
	CHECK_RC_ASSERT(bst_root, NULL);
	return rc;

}

/*
 * This test case tests deletion of leaf node.
 */
int bst_test_case5()
{

	int i;

	for (i = 7; i > 0; i--)
	{

		bst_insert(bst_root, &i, sizeof(int), bst_int_data_compare);
		CHECK_RC_ASSERT((bst_root == NULL), 0);

	}

	bst_inorder_traversal(bst_root, bst_int_node_printer);

	for (i = 1; i < 8; i++)
	{

		bst_delete_node(bst_root, &i, sizeof(int), bst_int_data_compare);
		printf("After deleting %d\n", i);
		bst_inorder_traversal(bst_root, bst_int_node_printer);

	}

	bst_root = NULL;
	CHECK_RC_ASSERT(bst_root, NULL);

	return EOK;

}

/*
 * This test case tests deletion of nodes with single left children
 */
int bst_test_case6()
{

	int rc = EOK;
	int i;

	for (i = 0; i < 7; i++)
	{

		bst_insert(bst_root, &i, sizeof(int), bst_int_data_compare);
		CHECK_RC_ASSERT((bst_root == NULL), 0);

	}

	bst_inorder_traversal(bst_root, bst_int_node_printer);

	for (i = 1; i < 6; i++)
	{

		bst_delete_node(bst_root, &i, sizeof(int), bst_int_data_compare);
		printf("After deleting %d\n", i);
		bst_inorder_traversal(bst_root, bst_int_node_printer);

	}

	i = 0;
	bst_delete_node(bst_root, &i, sizeof(int), bst_int_data_compare);
	printf("After deleting %d\n", i);
	bst_inorder_traversal(bst_root, bst_int_node_printer);

	i = 6;
	bst_delete_node(bst_root, &i, sizeof(int), bst_int_data_compare);
	printf("After deleting %d\n", i);
	bst_inorder_traversal(bst_root, bst_int_node_printer);

	bst_root = NULL;
	return EOK;

}

/*
 * This test case tests deletion of nodes with single right children
 */
int bst_test_case7()
{

	int rc = EOK;
	int i;

	bst_root = NULL;
	for (i = 7; i > 0; i--)
	{

		bst_insert(bst_root, &i, sizeof(int), bst_int_data_compare);
		CHECK_RC_ASSERT((bst_root == NULL), 0);

	}

	bst_inorder_traversal(bst_root, bst_int_node_printer);

	for (i = 6; i > 1; i--)
	{

		bst_delete_node(bst_root, &i, sizeof(int), bst_int_data_compare);
		printf("After deleting %d\n", i);
		bst_inorder_traversal(bst_root, bst_int_node_printer);

	}

	i = 7;
	bst_delete_node(bst_root, &i, sizeof(int), bst_int_data_compare);
	printf("After deleting %d\n", i);
	bst_inorder_traversal(bst_root, bst_int_node_printer);

	i = 1;
	bst_delete_node(bst_root, &i, sizeof(int), bst_int_data_compare);
	printf("After deleting %d\n", i);
	bst_inorder_traversal(bst_root, bst_int_node_printer);

	bst_root = NULL;
	return EOK;

}

/*
 * This test case tests deletion of node having both the children
 */
int bst_test_case8()
{

	int rc = EOK;
	int i;
	int data[7] ={10, 5, 1, 7, 40, 50};

	for (i = 0; i < 7; i++)
	{

		bst_insert(bst_root, &(data[i]), sizeof(int), bst_int_data_compare);
		CHECK_RC_ASSERT((bst_root == NULL), 0);

	}

	bst_inorder_traversal(bst_root, bst_int_node_printer);

	i = 5;
	bst_delete_node(bst_root, &i, sizeof(int), bst_int_data_compare);
	printf("After deleting %d\n", i);
	bst_inorder_traversal(bst_root, bst_int_node_printer);

	bst_destroy_tree(bst_root);
	bst_root = NULL;
	CHECK_RC_ASSERT(bst_root, NULL);
	return EOK;

}

int bst_tc_execute()
{

	int rc = EOK;

	rc = bst_test_case1();
	rc = bst_test_case2();
	rc = bst_test_case3();
	rc = bst_test_case4();
	rc = bst_test_case5();
	rc = bst_test_case6();
	rc = bst_test_case7();
	rc = bst_test_case8();

	return rc;

}

