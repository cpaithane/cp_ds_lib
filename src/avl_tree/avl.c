#include "avl.h"

/*
 * This is root node of BST initialized to NULL.
 */
avl_st *avl_root = NULL;

/*
 * This function returns the height at which node is present.
 */
unsigned int bst_get_node_height(avl_st *node)
{

	unsigned int height = 0;
	if (node)
	{
		height = node->bst_height;
	}

	return height;

}

/*
 * This function initializes node, pointers etc.
 */
void avl_initialize_node(avl_st *node, void *data, size_t len)
{

	memcpy(node->bst_data, data, len);
	node->bst_left = NULL;
	node->bst_right = NULL;
	node->bst_height = AVL_MIN_HEIGHT;

}

/*
 * This function allocates bst node.
 */
avl_st *avl_alloc_node(void *data, size_t len)
{

	avl_st *node = NULL;
	node = (avl_st *)malloc(sizeof(avl_st));
	memset(node, 0, sizeof(avl_st));

	node->bst_data = malloc(len);
	memset(node->bst_data, 0, len);
	avl_initialize_node(node, data, len);

	return node;

}

/*
 * This function deallocates node and data inside it.
 */
void avl_dealloc_node(avl_st **nodep)
{

	avl_st *node = *nodep;
	free(node->bst_data);
	free(node);
	*nodep = NULL;

}

/*
 * This function will update the node height after rebalancing.
 */
void bst_update_node_height(avl_st *root)
{

	unsigned int lheight = bst_get_node_height(root->bst_left);
	unsigned int rheight = bst_get_node_height(root->bst_right);
	root->bst_height = (1 + (MAX(lheight, rheight)));

}

/*
 * This function handles following special case of skewed left BST.
 * 
 * Lets handle a special case here:
 * 	root		 T1
 * 	/		/  \
 *     T1     ===>>    T2  root
 *     /
 *    T2
 */
avl_st *bst_handle_spl_right_rotate(avl_st *root)
{

	void *tmp = NULL;
	avl_st *T1 = root->bst_left;
	avl_st *T2 = T1->bst_left;
	CHECK_RC_ASSERT((T2 == NULL), 0);

	/*
	 * Swap bst_data pointers between root and T1.
	 */
	tmp = root->bst_data;
	root->bst_data = T1->bst_data;
	T1->bst_data = tmp;

	root->bst_left = T2;
	root->bst_right = T1;

	/*
	 * There could scenario that T1 could have right child/subtree.
	 */
	if (T1->bst_right)
	{

		T1->bst_left = T1->bst_right;
		T1->bst_right = NULL;

	}
	else
	{
		T1->bst_left = NULL;		
	}

	/*
	 * Now, update the heights
	 */
	bst_update_node_height(T1);
	bst_update_node_height(T2);
	bst_update_node_height(root);

	return root;

}

/*
 * This function does all the needful to rotate a subtree to left.
 * 		root			right
 *		/   \			/   \
 *		T1  right   ===>>>    root  T3
 *		    /   \	      /  \
 *		   T2   T3	      T1  T2
 */
avl_st* bst_left_rotate(avl_st *root)
{

	avl_st *T1 = root->bst_left;
	avl_st *right = root->bst_right;
	avl_st *T2 = NULL;
	avl_st *T3 = NULL;

	/*
	 * Let's handle special case here.
	 */
	if (right == NULL)
	{
		return (bst_handle_spl_left_rotate(root));
	}

	T2 = right->bst_left;
	T3 = right->bst_right;

	/*
	 * Perform rotation as shown in figure
	 */
	right->bst_left = root;
	root->bst_right = T2;

	/*
	 * Update the heights
	 */
	bst_update_node_height(root);
	bst_update_node_height(right);

	/*
	 * If rotation is targetted for bst_root node, then bst_root pointer should be 
	 * changed to right pointer. Diagram proves that right will be a new bst_root 
	 * in this case.
	 */
	if (root == avl_root)
	{
		avl_root = right;
	}

	return right;

}

/*
 * This function handles special case of right skewedness.
 *
 * 	root			 T1
 *	  \			/  \
 *         T1	===>>>        root  T2
 *          \
 *          T2
 */
avl_st *bst_handle_spl_left_rotate(avl_st *root)
{

	void *tmp = NULL;
	avl_st *T1 = root->bst_right;
	avl_st *T2 = T1->bst_right;

	CHECK_RC_ASSERT((T2 == NULL), 0);

	/*
	 * Swap bst_data pointers between root and T1.
	 */
	tmp = root->bst_data;
	root->bst_data = T1->bst_data;
	T1->bst_data = tmp;

	root->bst_right = T2;
	root->bst_left = T1;

	/*
	 * Handle a scenario where left subtree is available for T1.
	 */
	if (T1->bst_left)
	{
		T1->bst_right = T1->bst_left;
		T1->bst_left = NULL;
	}
	else
	{
		T1->bst_right = NULL;
	}

	/*
	 * Update the heights
	 */
	bst_update_node_height(T1);
	bst_update_node_height(T2);
	bst_update_node_height(root);

	return root;

}

/*
 * This function does all the needful to rotate a subtree to right.
 * 		root			left
 *		/   \			/   \
 *	      left   T3   ===>>>       T1   root
 *	      /  \	    		    /  \
 *	      T1 T2			   T2  T3
 */
avl_st* bst_right_rotate(avl_st *root)
{

	avl_st *T3 = root->bst_right;

	/*
	 * Let's handle a special case here.
	 */
	if (T3 == NULL)
	{
		return (bst_handle_spl_right_rotate(root));
	}

	avl_st *left = root->bst_left;
	avl_st *T1 = left->bst_left;
	avl_st *T2 = left->bst_right;

	/*
	 * Perform rotation as shown in figure
	 */
	left->bst_right = root;
	root->bst_left = T2;

	/*
	 * Update the heights
	 */
	bst_update_node_height(root);
	bst_update_node_height(left);

	/*
	 * If rotation is targetted for bst_root node, then bst_root pointer should be 
	 * changed to left pointer. Diagram proves that left will be a new bst_root 
	 * in this case.
	 */
	if (root == avl_root)
	{
		avl_root = left;
	}

	return left;

}

/*
 * This function returns balance factor of the node.
 */
int bst_get_balance_factor(avl_st *node)
{

	int balance_factor = 0;
	if (node)
	{

		balance_factor = 
			bst_get_node_height(node->bst_left) -
			bst_get_node_height(node->bst_right);

	}

	return balance_factor;

}

/*
 * This function does balancing of bst to convert it to AVL.
 * There are 4 cases in which balancing is done listed in the implementation.
 */
avl_st *bst_do_balance(avl_st *root, 
		       void *data, 
		       bst_data_compare_t compare)
{

	int balance_factor;

	balance_factor = bst_get_balance_factor(root);

	/*
	 * Left left case
	 */
	if ((balance_factor > 1) && 
	    (compare(data, root->bst_left->bst_data) == FIRST_LESS))
	{
		return (bst_right_rotate(root));
	}

	/*
	 * Right right case
	 */
	if ((balance_factor < -1) && 
	    (compare(data, root->bst_right->bst_data) == FIRST_GREATER))
	{
		return (bst_left_rotate(root));
	}

	/*
	 * Left right case
	 */
	if ((balance_factor > 1) && 
	    (compare(data, root->bst_left->bst_data) == FIRST_GREATER))
	{

		root->bst_left = bst_left_rotate(root->bst_left);
		return (bst_right_rotate(root));

	}

	/*
	 * Right left case
	 */
	if ((balance_factor < -1) && 
	    (compare(data, root->bst_right->bst_data) == FIRST_LESS))
	{

		root->bst_right = bst_left_rotate(root->bst_right);
		return (bst_left_rotate(root));

	}

	/*
	 * Else, pointer is unchanged. Return root.
	 */
	return root;

}

/*
 * This function inserts data into bst. compare function pointer can be overriden. 
 */
avl_st *avl_insert(avl_st *root, void *data, size_t len, bst_data_compare_t compare)
{

	int rc = EOK;

	/*
	 * If root node of btree is NULL, allocate and set root node.
	 */
	if (root == NULL)
	{

		root = avl_alloc_node(data, len);

		/*
		 * If global root node of BST is NULL, then it is first node 
		 * allocation.
		 */
		if (avl_root == NULL)
		{
			avl_root = root;
		}

		return root;

	}

	rc = compare(data, root->bst_data);

	/*
	 * If data to be inserted is greater, then look into right subtree. Else, look into
	 * left subtree.
	 * As per property of BST, no two identical nodes present in BST.
	 */
	if (rc == FIRST_GREATER)
	{

		root->bst_right = avl_insert(root->bst_right, 
					     data, len, 
					     compare);

	}
	else if (rc == FIRST_LESS)
	{

		root->bst_left = avl_insert(root->bst_left, 
					     data, len, 
					     compare);

	}
	else
	{
		/*
		 * Control should not reach here. Messy stuff.
		 */ 
		CHECK_RC_ASSERT(rc == IDENTICAL, 1);
		return NULL;

	}

	bst_update_node_height(root);
	return (bst_do_balance(root, data, compare));

}

void avl_inorder_traversal(avl_st *root, avl_node_printer_t avl_node_printer)
{

	/*
	 * If root is NULL, return.
	 */
	if (root == NULL)
	{
		return;
	}

	/*
	 * traverse to the left subtree
	 */
	avl_inorder_traversal(root->bst_left, avl_node_printer);

	/*
	 * print the node.
	 */
	avl_node_printer(root);

	/*
	 * traverse to the right subtree
	 */
	avl_inorder_traversal(root->bst_right, avl_node_printer);

}

/*
 * This function finds a node which has minimum value in tree. 
 */
avl_st *avl_get_min_value(avl_st *root)
{

	avl_st *in_suc = NULL;
	avl_st *right_subtree = root;

	/*
	 * Find out first inorder successor of the node.
	 */
	while (right_subtree->bst_left != NULL)
	{
		right_subtree = right_subtree->bst_left;
	}

	in_suc = right_subtree;
	CHECK_RC_ASSERT((in_suc == NULL), 0);
	return in_suc;

}

/*
 * This function does all balancing work post-delete in bottom-up fashion. 
 */
avl_st *avl_do_balance_after_delete_internal(avl_st *root)
{

	int balance_factor, lbalance_factor, rbalance_factor;

	if (root == NULL)
	{
		return root;
	}
	
	balance_factor = bst_get_balance_factor(root);
	lbalance_factor = bst_get_balance_factor(root->bst_left);
	rbalance_factor = bst_get_balance_factor(root->bst_right);

	/*
	 * Left left case
	 */
	if ((balance_factor > 1) && (lbalance_factor >= 0))
	{
		return (bst_right_rotate(root));
	}

	/*
	 * Right right case
	 */
	if ((balance_factor < -1) && (rbalance_factor <= 0))
	{
		return (bst_left_rotate(root));
	}

	/*
	 * Left right case
	 */
	if ((balance_factor > 1) && (lbalance_factor < 0))
	{

		root->bst_left = bst_left_rotate(root->bst_left);
		return (bst_right_rotate(root));

	}

	/*
	 * Right left case
	 */
	if ((balance_factor < -1) && (rbalance_factor > 0)) 
	{

		root->bst_right = bst_left_rotate(root->bst_right);
		return (bst_left_rotate(root));

	}

	/*
	 * Else, pointer is unchanged. Return root.
	 */
	return root;

}

/*
 * This function deletes the node which contains specified value
 */
avl_st *avl_delete_node(avl_st *root, 
			void *data, 
			size_t len, 
			bst_data_compare_t compare)
{

	int rc = EOK;
	avl_st *temp;

	/*
	 * First traverse to the node of deletion.
	 */
	
	CHECK_RC_ASSERT((data == NULL), 0);

	if (root == NULL)
	{
		return root;
	}

	rc = compare(root->bst_data, data);
	if (rc == FIRST_GREATER)
	{

		root->bst_left = avl_delete_node(root->bst_left,
						 data, len, compare);

	}
	else if (rc == FIRST_LESS)
	{

		root->bst_right = avl_delete_node(root->bst_right,
						 data, len, compare);

	}
	else
	{

		CHECK_RC_ASSERT((root == NULL), 0);

		if ((root->bst_left == NULL) || 
		    (root->bst_right == NULL))
		{

			temp = root->bst_left ? root->bst_left : root->bst_right;

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

				memcpy(root->bst_data, temp->bst_data, len);
				root->bst_left = NULL;
				root->bst_right = NULL;

			}

			if (avl_root == temp)
			{
				avl_root = NULL;
			}
			avl_dealloc_node(&temp);
		}
		else
		{

			temp = avl_get_min_value(root->bst_right);
			memcpy(root->bst_data, temp->bst_data, len);
			root->bst_right = avl_delete_node(root->bst_right,
					  root->bst_data, len, compare);

		}

	}

	if (root == NULL)
	{
		return root;
	}

	bst_update_node_height(root);
	return (avl_do_balance_after_delete_internal(root));

}

/*
 * This function checks whether balance factor is from -1 to 1.
 */
int avl_check_sanity(avl_st *root)
{

	int balance_factor = 0;

	balance_factor = abs(bst_get_balance_factor(root));
	if (balance_factor == 0 || 
	    balance_factor == 1)
	{
		return 1;
	}
	else
	{
		CHECK_RC_ASSERT(balance_factor, 1);
	}

	avl_check_sanity(root->bst_left);
	avl_check_sanity(root->bst_right);
	return 1;

}

/*
 * This function prints integer data inside node
 */
void avl_int_node_printer(const avl_st *node)
{

	int *data;
	printf("\n");
	if (node)
	{

		data = (int *)node->bst_data;
		printf("%d\t", *data);

	}
	printf("\n");

}

/*
 * This function destroys tree rooted at root
 */
void avl_destroy_tree(avl_st *root)
{

	if (root == NULL)
	{
		return;
	}

	avl_destroy_tree(root->bst_left);
	avl_destroy_tree(root->bst_right);
	avl_dealloc_node(&root);

}

