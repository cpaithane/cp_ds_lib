#include "rb_tree.h"

extern rb_tree_st *rb_tree_root;

/*
 * This function deletes the node which contains specified value
 */
rb_tree_st *rb_tree_delete_node(rb_tree_st *root,
			void *data, 
			size_t len, 
			common_data_compare_t compare)
{

	int rc = EOK;
	rb_tree_st *temp;

	/*
	 * First traverse to the node of deletion.
	 */
	CHECK_RC_ASSERT((data == NULL), 0);

	if (root == NULL)
	{
		return root;
	}

	rc = compare(root->rb_tree_data, data);
	if (rc == FIRST_GREATER)
	{

		root->rb_tree_left = rb_tree_delete_node(
					root->rb_tree_left,
					data, len, compare);

	}
	else if (rc == FIRST_LESS)
	{

		root->rb_tree_right = rb_tree_delete_node(
					root->rb_tree_right,
					data, len, compare);

	}
	else
	{

		CHECK_RC_ASSERT((root == NULL), 0);

		if ((root->rb_tree_left == NULL) || 
		    (root->rb_tree_right == NULL))
		{

			temp = root->rb_tree_left ? 
				root->rb_tree_left : root->rb_tree_right;

			/*
			 * No child case.
			 */
			if (temp == NULL)
			{

				temp = root;
				root = NULL;
			}
			else
			{

				memcpy(root->rb_tree_data, temp->rb_tree_data, len);
				root->rb_tree_left = NULL;
				root->rb_tree_right = NULL;

			}

			if (rb_tree_root == temp)
			{
				rb_tree_root = NULL;
			}
			rb_tree_dealloc_node(&temp);

		}
		else
		{

			temp = rb_tree_get_min_value(root->rb_tree_right);
			memcpy(root->rb_tree_data, temp->rb_tree_data, len);
			root->rb_tree_right = rb_tree_delete_node(
						root->rb_tree_right,
						root->rb_tree_data, len, compare);

		}

	}

	if (root == NULL)
	{
		return root;
	}

	return root;

}

