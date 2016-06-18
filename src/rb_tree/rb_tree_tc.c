#include "rb_tree.h"

extern rb_tree_st *rb_tree_root;
extern max_nr_nodes;

/*
 * This test case tests insertion with 50 nodes in rightmost side.
 * Mostly, this test case tests case 1 and case 3 violations.
 */
int rb_tree_test_case1()
{

	int rc = EOK;
	int i;

	max_nr_nodes = 50;
	for (i = 0; i < 50; i++)
	{

		rb_tree_insert(
				rb_tree_root,
				&i, sizeof(int),
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
 * This test case tests insertion with 50 nodes in leftmost side.
 */
int rb_tree_test_case2()
{

	int rc = EOK;
	int i;

	max_nr_nodes = 0;
	for (i = 50; i > 0; i--)
	{

		rb_tree_insert(
				rb_tree_root,
				&i,
				sizeof(int),
				common_int_data_compare);
		CHECK_RC_ASSERT((rb_tree_root == NULL), 0);
		max_nr_nodes++;

	}

	rb_tree_inorder_traversal(rb_tree_root, rb_tree_int_node_printer);
	rb_tree_destroy_tree(rb_tree_root);
	rb_tree_root = NULL;
	CHECK_RC_ASSERT(rb_tree_root, NULL);

	return rc;

}

/*
 * This function inserts random data inside BST. Mostly this tests case 2 
 * violations.
 */
int rb_tree_test_case3()
{

	int rc = EOK;
	int i;
	int data[16] =
	{10, 1000, 20, 2000, 30, 3000, 40, 4000, 50, 5000, 60, 
	6000, 70, 7000, 80, 8000};

	max_nr_nodes = 16;
	for (i = 0; i < 16; i++)
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

	for (i = 15; i >= 0; i--)
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

#ifdef DELETE_SUPPORT

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

#endif

int rb_tree_tc_execute()
{

	int rc = EOK;
	rc = rb_tree_test_case1();
	rc = rb_tree_test_case2();
	rc = rb_tree_test_case3();
#ifdef DELETE_SUPPORT
	rc = rb_tree_test_case4();
	rc = rb_tree_test_case5();
	rc = rb_tree_test_case6();
	rc = rb_tree_test_case7();
	rc = rb_tree_test_case8();
#endif

	return rc;

}

