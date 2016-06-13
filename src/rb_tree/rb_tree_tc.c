#include "rb_tree.h"

extern rb_tree_st *rb_tree_root;

/*
 * This test case tests insertion with 7 nodes in rightmost side.
 */
int rb_tree_test_case1()
{

	int rc = EOK;
	int i;
	int data[7] = {100, 10, 200, 20, 400, 30, 600};

	for (i = 0; i < 7; i++)
	{

		rb_tree_insert(rb_tree_root, &i, sizeof(int), common_int_data_compare);
		CHECK_RC_ASSERT((rb_tree_root == NULL), 0);

	}

	rb_tree_inorder_traversal(rb_tree_root, rb_tree_int_node_printer);
	rb_tree_destroy_tree(rb_tree_root);
	rb_tree_root = NULL;
	CHECK_RC_ASSERT(rb_tree_root, NULL);

	return rc;

}

/*
 * This test case tests insertion with 7 nodes in leftmost side.
 */
int rb_tree_test_case2()
{

	int rc = EOK;
	int i;
	int data[7] = {100, 10, 200, 20, 400, 30, 600};

	for (i = 10; i > 0; i--)
	{

		rb_tree_insert(
				rb_tree_root,
				&i,
				sizeof(int),
				common_int_data_compare);
		CHECK_RC_ASSERT((rb_tree_root == NULL), 0);

	}

	rb_tree_inorder_traversal(rb_tree_root, rb_tree_int_node_printer);
	rb_tree_destroy_tree(rb_tree_root);
	rb_tree_root = NULL;
	CHECK_RC_ASSERT(rb_tree_root, NULL);

	return rc;

}

/*
 * This function inserts random data inside BST.
 */
int rb_tree_test_case3()
{

	int rc = EOK;
	int i;
	int data[7] = {100, 10, 200, 20, 400, 30, 600};

	for (i = 0; i < 7; i++)
	{

		rb_tree_insert(
				rb_tree_root,
				&(data[i]),
				sizeof(int),
				common_int_data_compare);
		CHECK_RC_ASSERT((rb_tree_root == NULL), 0);

	}

	rb_tree_inorder_traversal(rb_tree_root, rb_tree_int_node_printer);
	rb_tree_destroy_tree(rb_tree_root);
	rb_tree_root = NULL;

	return rc;

}

/*
 * This test case tests deletion of leaf node.
 */
int rb_tree_test_case4()
{

	int rc = EOK;
	int i;

	for (i = 0; i < 7; i++)
	{

		rb_tree_insert(rb_tree_root,
				&i,
				sizeof(int),
				common_int_data_compare);
		CHECK_RC_ASSERT((rb_tree_root == NULL), 0);

	}

	rb_tree_inorder_traversal(rb_tree_root, rb_tree_int_node_printer);

	for (i = 6; i >= 0; i--)
	{

		rb_tree_delete_node(rb_tree_root, &i,
				    sizeof(int), common_int_data_compare);
		printf("After deleting %d\n", i);
		rb_tree_inorder_traversal(rb_tree_root, rb_tree_int_node_printer);

	}

	rb_tree_root = NULL;
	CHECK_RC_ASSERT(rb_tree_root, NULL);
	return rc;

}

/*
 * This test case tests deletion of leaf node.
 */
int rb_tree_test_case5()
{

	int i;

	for (i = 7; i > 0; i--)
	{

		rb_tree_insert(rb_tree_root,
				&i,
				sizeof(int),
				common_int_data_compare);
		CHECK_RC_ASSERT((rb_tree_root == NULL), 0);

	}

	rb_tree_inorder_traversal(rb_tree_root, rb_tree_int_node_printer);

	for (i = 1; i < 8; i++)
	{

		rb_tree_delete_node(rb_tree_root, &i,
				    sizeof(int), common_int_data_compare);
		printf("After deleting %d\n", i);
		rb_tree_inorder_traversal(rb_tree_root, rb_tree_int_node_printer);

	}

	rb_tree_root = NULL;
	CHECK_RC_ASSERT(rb_tree_root, NULL);

	return EOK;

}

/*
 * This test case tests deletion of nodes with single left children
 */
int rb_tree_test_case6()
{

	int rc = EOK;
	int i;

	for (i = 0; i < 7; i++)
	{

		rb_tree_insert(rb_tree_root,
				&i,
				sizeof(int),
				common_int_data_compare);
		CHECK_RC_ASSERT((rb_tree_root == NULL), 0);

	}

	rb_tree_inorder_traversal(rb_tree_root, rb_tree_int_node_printer);

	for (i = 1; i < 6; i++)
	{

		rb_tree_delete_node(rb_tree_root, &i,
				    sizeof(int), common_int_data_compare);
		printf("After deleting %d\n", i);
		rb_tree_inorder_traversal(rb_tree_root, rb_tree_int_node_printer);

	}

	i = 0;
	rb_tree_delete_node(rb_tree_root, &i,
			    sizeof(int), common_int_data_compare);
	printf("After deleting %d\n", i);
	rb_tree_inorder_traversal(rb_tree_root, rb_tree_int_node_printer);

	i = 6;
	rb_tree_delete_node(rb_tree_root, &i, sizeof(int), common_int_data_compare);
	printf("After deleting %d\n", i);
	rb_tree_inorder_traversal(rb_tree_root, rb_tree_int_node_printer);

	rb_tree_root = NULL;
	return EOK;

}

/*
 * This test case tests deletion of nodes with single right children
 */
int rb_tree_test_case7()
{

	int rc = EOK;
	int i;

	rb_tree_root = NULL;
	for (i = 7; i > 0; i--)
	{

		rb_tree_insert(rb_tree_root,
				&i,
				sizeof(int),
				common_int_data_compare);
		CHECK_RC_ASSERT((rb_tree_root == NULL), 0);

	}

	rb_tree_inorder_traversal(rb_tree_root, rb_tree_int_node_printer);

	for (i = 6; i > 1; i--)
	{

		rb_tree_delete_node(rb_tree_root, &i,
				    sizeof(int), common_int_data_compare);
		printf("After deleting %d\n", i);
		rb_tree_inorder_traversal(rb_tree_root, rb_tree_int_node_printer);

	}

	i = 7;
	rb_tree_delete_node(rb_tree_root, &i, sizeof(int), common_int_data_compare);
	printf("After deleting %d\n", i);
	rb_tree_inorder_traversal(rb_tree_root, rb_tree_int_node_printer);

	i = 1;
	rb_tree_delete_node(rb_tree_root, &i, sizeof(int), common_int_data_compare);
	printf("After deleting %d\n", i);
	rb_tree_inorder_traversal(rb_tree_root, rb_tree_int_node_printer);

	rb_tree_root = NULL;
	return EOK;

}

/*
 * This test case tests deletion of node having both the children
 */
int rb_tree_test_case8()
{

	int rc = EOK;
	int i;
	int data[7] ={10, 5, 1, 7, 40, 50};

	for (i = 0; i < 7; i++)
	{

		rb_tree_insert(rb_tree_root,
				&(data[i]),
				sizeof(int),
				common_int_data_compare);
		CHECK_RC_ASSERT((rb_tree_root == NULL), 0);

	}

	rb_tree_inorder_traversal(rb_tree_root, rb_tree_int_node_printer);

	i = 5;
	rb_tree_delete_node(rb_tree_root, &i, sizeof(int), common_int_data_compare);
	printf("After deleting %d\n", i);
	rb_tree_inorder_traversal(rb_tree_root, rb_tree_int_node_printer);

	rb_tree_destroy_tree(rb_tree_root);
	rb_tree_root = NULL;
	CHECK_RC_ASSERT(rb_tree_root, NULL);
	return EOK;

}

int rb_tree_tc_execute()
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

