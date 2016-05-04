#include "avl.h"

extern avl_st *avl_root;
/*
 * This test case tests insertion with 7 nodes in rightmost side.
 */
int avl_test_case1()
{

	int rc = EOK;
	int i;
	int data[7] = {100, 10, 200, 20, 400, 30, 600};

	for (i = 0; i < 7; i++)
	{

		avl_insert(avl_root, &i, sizeof(int), common_int_data_compare);
		avl_check_sanity(avl_root);

	}

	avl_inorder_traversal(avl_root, avl_int_node_printer);
	avl_destroy_tree(avl_root);
	avl_root = NULL;
	CHECK_RC_ASSERT(avl_root, NULL);

	return rc;

}

/*
 * This test case tests insertion with 7 nodes in leftmost side.
 */
int avl_test_case2()
{

	int rc = EOK;
	int i;
	int data[7] = {100, 10, 200, 20, 400, 30, 600};

	for (i = 10; i > 0; i--)
	{

		avl_insert(avl_root, &i, sizeof(int), common_int_data_compare);
		avl_check_sanity(avl_root);

	}

	avl_inorder_traversal(avl_root, avl_int_node_printer);
	avl_destroy_tree(avl_root);
	avl_root = NULL;
	CHECK_RC_ASSERT(avl_root, NULL);

	return rc;

}

/*
 * This function inserts random data inside BST.
 */
int avl_test_case3()
{

	int rc = EOK;
	int i;
	int data[7] = {100, 10, 200, 20, 400, 30, 600};

	for (i = 0; i < 7; i++)
	{

		avl_insert(avl_root, &(data[i]), sizeof(int), common_int_data_compare);
		CHECK_RC_ASSERT((avl_root == NULL), 0);
		avl_check_sanity(avl_root);

	}

	avl_inorder_traversal(avl_root, avl_int_node_printer);
	avl_destroy_tree(avl_root);
	avl_root = NULL;
	CHECK_RC_ASSERT(avl_root, NULL);

	return rc;

}

/*
 * This test case tests deletion of leaf node.
 */
int avl_test_case4()
{

	int rc = EOK;
	int i;

	for (i = 0; i < 7; i++)
	{

		avl_insert(avl_root, &i, sizeof(int), common_int_data_compare);
		CHECK_RC_ASSERT((avl_root == NULL), 0);
		avl_check_sanity(avl_root);

	}

	avl_inorder_traversal(avl_root, avl_int_node_printer);

	for (i = 6; i >= 0; i--)
	{

		avl_delete_node(avl_root, &i, sizeof(int), common_int_data_compare);
		printf("After deleting %d\n", i);
		avl_inorder_traversal(avl_root, avl_int_node_printer);
		avl_check_sanity(avl_root);

	}

	avl_root = NULL;
	CHECK_RC_ASSERT(avl_root, NULL);
	return rc;

}

/*
 * This test case tests deletion of leaf node.
 */
int avl_test_case5()
{

	int i;

	for (i = 7; i > 0; i--)
	{

		avl_insert(avl_root, &i, sizeof(int), common_int_data_compare);
		CHECK_RC_ASSERT((avl_root == NULL), 0);
		avl_check_sanity(avl_root);

	}

	avl_inorder_traversal(avl_root, avl_int_node_printer);

	for (i = 1; i < 8; i++)
	{

		avl_delete_node(avl_root, &i, sizeof(int), common_int_data_compare);
		printf("After deleting %d\n", i);
		avl_inorder_traversal(avl_root, avl_int_node_printer);
		avl_check_sanity(avl_root);

	}

	avl_root = NULL;
	CHECK_RC_ASSERT(avl_root, NULL);

	return EOK;

}

/*
 * This test case tests deletion of nodes with single left children
 */
int avl_test_case6()
{

	int rc = EOK;
	int i;

	for (i = 0; i < 7; i++)
	{

		avl_insert(avl_root, &i, sizeof(int), common_int_data_compare);
		CHECK_RC_ASSERT((avl_root == NULL), 0);

	}

	avl_inorder_traversal(avl_root, avl_int_node_printer);

	for (i = 1; i < 6; i++)
	{

		avl_delete_node(avl_root, &i, sizeof(int), common_int_data_compare);
		printf("After deleting %d\n", i);
		avl_inorder_traversal(avl_root, avl_int_node_printer);

	}

	i = 0;
	avl_delete_node(avl_root, &i, sizeof(int), common_int_data_compare);
	printf("After deleting %d\n", i);
	avl_inorder_traversal(avl_root, avl_int_node_printer);

	i = 6;
	avl_delete_node(avl_root, &i, sizeof(int), common_int_data_compare);
	printf("After deleting %d\n", i);
	avl_inorder_traversal(avl_root, avl_int_node_printer);

	avl_root = NULL;
	return EOK;

}

/*
 * This test case tests deletion of nodes with single right children
 */
int avl_test_case7()
{

	int rc = EOK;
	int i;

	avl_root = NULL;
	for (i = 7; i > 0; i--)
	{

		avl_insert(avl_root, &i, sizeof(int), common_int_data_compare);
		CHECK_RC_ASSERT((avl_root == NULL), 0);

	}

	avl_inorder_traversal(avl_root, avl_int_node_printer);

	for (i = 6; i > 1; i--)
	{

		avl_delete_node(avl_root, &i, sizeof(int), common_int_data_compare);
		printf("After deleting %d\n", i);
		avl_inorder_traversal(avl_root, avl_int_node_printer);

	}

	i = 7;
	avl_delete_node(avl_root, &i, sizeof(int), common_int_data_compare);
	printf("After deleting %d\n", i);
	avl_inorder_traversal(avl_root, avl_int_node_printer);

	i = 1;
	avl_delete_node(avl_root, &i, sizeof(int), common_int_data_compare);
	printf("After deleting %d\n", i);
	avl_inorder_traversal(avl_root, avl_int_node_printer);

	avl_root = NULL;
	return EOK;

}

/*
 * This test case tests deletion of node having both the children
 */
int avl_test_case8()
{

	int rc = EOK;
	int i;
	int data[7] ={10, 5, 1, 7, 40, 50};

	for (i = 0; i < 7; i++)
	{

		avl_insert(avl_root, &(data[i]), sizeof(int), common_int_data_compare);
		CHECK_RC_ASSERT((avl_root == NULL), 0);

	}

	avl_inorder_traversal(avl_root, avl_int_node_printer);

	i = 5;
	avl_delete_node(avl_root, &i, sizeof(int), common_int_data_compare);
	printf("After deleting %d\n", i);
	avl_inorder_traversal(avl_root, avl_int_node_printer);

	avl_destroy_tree(avl_root);
	avl_root = NULL;
	CHECK_RC_ASSERT(avl_root, NULL);
	return EOK;

}

int avl_tc_execute()
{

	int rc = EOK;

	rc = avl_test_case1();
	rc = avl_test_case2();
	rc = avl_test_case3();
	rc = avl_test_case4();
	rc = avl_test_case5();
	rc = avl_test_case6();
	rc = avl_test_case7();
	rc = avl_test_case8();

	return rc;

}

