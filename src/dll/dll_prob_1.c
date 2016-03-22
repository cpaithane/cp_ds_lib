/*
 * Convert a tree in dll.
 */

#include "../bst/bst.h"
#include "dll.h"

extern bst_st *bst_root;

/*
 * This is the main function which implements core logic of converstion.
 */
bst_st *dll_prob_1_tree_to_list(bst_st *root)
{

	if (root == NULL)
	{
		return NULL;
	}

	/*
	 * Convert in inorder fashion. Traverse to the leftmost children.
	 */
	dll_prob_1_tree_to_list(root->bst_left);

	/*
	 * Convert the node into dll node.
	 */
	if (root->bst_left)
	{
		root->bst_left->bst_right = root;
	}

	/*
	 * Go to right.
	 */
	dll_prob_1_tree_to_list(root->bst_right);

	/*
	 * Convert the node into dll node.
	 */
	if (root->bst_right)
	{

		root->bst_right->bst_left = root;
		return root->bst_right;

	}
	else
	{
		return root;
	}

}

int dll_prob_1()
{

	int rc = EOK;
	bst_st *root = NULL;

	int i;
	int data[7] = {100, 10, 200, 20, 400, 30, 600};

	for (i = 0; i < 7; i++)
	{

		bst_insert(bst_root, &(data[i]), sizeof(int), bst_int_data_compare);
		CHECK_RC_ASSERT((bst_root == NULL), 0);

	}

	root = dll_prob_1_tree_to_list(bst_root);

	printf("Printing in straight = \n");
	while (root)
	{

		printf("%d--->", *(int *)root->bst_data);
		root = root->bst_right;

	}
	printf("\n");

	printf("Printing in reverse = \n");
	while (root)
	{

		printf("%d--->", *(int *)root->bst_data);
		root = root->bst_left;

	}
	printf("\n");
	return rc;

}

