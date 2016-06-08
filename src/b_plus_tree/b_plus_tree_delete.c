/*
 * This file contails all the APIs for deletion of key-value pair from B+ tree.
 */

#include "b_plus_tree_interface.h"

/*
 * This function deletes an item located at pe_position in leaf node and re-balances
 * b+ tree.
 */
int bplus_tree_delete_item(char *root_path,
			   bplus_tree_traverse_path_st *traverse_path)
{

	int rc = EOK;
	void *leaf_node = NULL;
	int i;
	block_head_st *block_head = NULL;

	leaf_node = bplus_tree_get_node_path(traverse_path, BTREE_LEAF_LEVEL);
	CHECK_RC_ASSERT((leaf_node == NULL), 0);

	block_head = bplus_tree_get_block_head(leaf_node);
	CHECK_RC_ASSERT((block_head == NULL), 0);

	/*
	 * Condition for rebalancing after deletion is that leaf node should have 
	 * less than MAX_ITEMS / 2 items. Otherwise, it would be simple delete op.
	 */
	if ((block_head->nr_items < (MAX_ITEMS / 2)) &&
	    (traverse_path->path_length))
	{

		rc = bplus_tree_rebalance(root_path,
					  traverse_path,
					  NULL, BPLUS_DELETE);
		return rc;

	}

	bplus_tree_simple_delete(traverse_path);
	return rc;

}

/*
 * This function handles a specific case in deletion where right sibling of a node
 * is not present.
 */
int bplus_tree_delete_handle_case1(bplus_tree_balance_st *tb)
{

	int i, rc = EOK;
	void *left_sib = NULL;
	void *node = NULL;
	bplus_tree_traverse_path_st *traverse_path = NULL;
	block_head_st *block_head_sib = NULL;
	block_head_st *block_head = NULL;
	uint16_t nr_items_sib = 0;
	uint16_t nr_items = 0;
	uint16_t nr_items_can_acc = 0;

	traverse_path = tb->tb_path;

	/*
	 * Do a simple delete first for the item in question.
	 */
	bplus_tree_simple_delete(traverse_path);

	/*
	 * Get the left sibling.
	 */
	left_sib = bplus_tree_get_left_sibling(tb, BTREE_LEAF_LEVEL);
	CHECK_RC_ASSERT((left_sib == NULL), 0);

	/*
	 * Get the leaf node in path.
	 */
	node = bplus_tree_get_node_path(traverse_path, BTREE_LEAF_LEVEL);

	/*
	 * Check how many items present in left sibling. If left sibling can
	 * accomodate all items, then merge the node with left sibling.
	 */
	block_head_sib = bplus_tree_get_block_head(left_sib);
	block_head = bplus_tree_get_block_head(node);
	nr_items_sib = block_head_sib->nr_items;
	nr_items = block_head->nr_items;

	/*
	 * nr_items_can_acc : No. of items can be accommodate in left sibling.
	 * If this is 0, then no flow can happen or
	 * Also, if no. of items in node is greater than MAX_ITEMS/2.
	 * If one of the three conditions holds true, return.
	 */
	nr_items_can_acc = (MAX_ITEMS - nr_items_sib);
	if ((nr_items_can_acc == 0) ||
		(nr_items >= (MAX_ITEMS / 2)) ||
		(nr_items_can_acc < nr_items))
	{
		return rc;
	}

	CHECK_RC_ASSERT((nr_items_can_acc >= nr_items), 1);

	/*
	 * See how many no. of items can left sibling accommodate. If it can 
	 * accommodate all then flow all to left sibling from node.
	 */
	for (i = 0; i < nr_items; i++)
	{
		bplus_tree_flow_item(node, left_sib, i);
	}

	/*
	 * Now, shift left key and disk child in parent.
	 * As items are flown from node to left sibling, parent's position of left
	 * sibling is one prior to node.
	 */
	rc = bplus_tree_delete_key(tb, NODE_TO_LEFT_SIB);
	return rc;

}

/*
 * This function handles a specific case in deletion where right sibling of a node
 * is present.
 */
int bplus_tree_delete_handle_case2(bplus_tree_balance_st *tb)
{

	int i, rc = EOK;
	void *right_sib = NULL;
	void *node = NULL;
	block_head_st *block_head_sib = NULL;
	block_head_st *block_head_par = NULL;
	uint16_t nr_items_sib = 0;
	uint16_t nr_items_to_flow = 0;
	bool handle_pos0 = FALSE;

	/*
	 * Left shift the node one by one.
	 */
	bplus_tree_simple_delete(tb->tb_path);

	/*
	 * Check if sufficient items are present in right sibling.
	 * If sufficient items are present, flow first item from right sibling to
	 * node.
	 */
	right_sib = bplus_tree_get_right_sibling(tb, BTREE_LEAF_LEVEL);
	block_head_sib = bplus_tree_get_block_head(right_sib);

	nr_items_sib = block_head_sib->nr_items;
	nr_items_to_flow = nr_items_sib;
	if (nr_items_sib >= (MAX_ITEMS / 2))
	{

		nr_items_to_flow = 1;
		handle_pos0 = TRUE;

	}

	node = bplus_tree_get_node_path(tb->tb_path, BTREE_LEAF_LEVEL);

	for (i = 0; i < nr_items_to_flow; i++)
	{
		bplus_tree_flow_item_left(right_sib, node, i);
	}

	/*
	 * As all items in sibling are flown to node, lets marks the sibling for 
	 * deletion.
	 */
	if (nr_items_to_flow > 1)
	{
		bplus_tree_mark_for_delete(block_head_sib);
	}

	/*
	 * If position of item in sibling is 0, handle it.
	 */
	if (handle_pos0 == TRUE)
	{

		/*
		 * Lets flow all the items from right sibling to the node and left
		 * shift.
		 */
		i = bplus_tree_shift_left(right_sib, 0, block_head_sib->nr_items);
		bplus_tree_delete_item_pos0(
					tb->tb_path,
					right_sib,
					TRUE);

	}
	else
	{

		/*
		 * As items are flown from right sibling to node, no need to adjust
		 * parent's position.
		 */
		rc = bplus_tree_delete_key(tb, RIGHT_SIB_TO_NODE);

	}

	return rc;

}

/*
 * This function is called from delete case_3_1. It returns child0 and child1.
 */
void bplus_tre_get_child_0_1(
			bplus_tree_balance_st *tb,
			void **child_node0,
			void **child_node1,
			char **new_root_node_path)
{

	int root_pos = 0;
	bplus_tree_traverse_path_st *traverse_path = NULL;

	traverse_path = tb->tb_path;
	root_pos = bplus_tree_get_pos_path(traverse_path,
					  (BTREE_LEAF_LEVEL + 2));
	traverse_path = tb->tb_path;

	if (root_pos == 0)
	{

		*child_node0 = bplus_tree_get_node_path(traverse_path,
					  (BTREE_LEAF_LEVEL + 1));

		*child_node1 = bplus_tree_get_right_sibling(tb,
					  (BTREE_LEAF_LEVEL + 1));

		*new_root_node_path = bplus_tree_get_pe_path_path(traverse_path,
					  (BTREE_LEAF_LEVEL + 1));

	}
	else
	{

		*child_node1 = bplus_tree_get_node_path(traverse_path,
					  (BTREE_LEAF_LEVEL + 1));

		*child_node0 = bplus_tree_get_left_sibling(tb,
					  (BTREE_LEAF_LEVEL + 1));

		*new_root_node_path = bplus_tree_get_left_sibling_path(tb,
					  (BTREE_LEAF_LEVEL + 1));

	}

}

/*
 * This function handles specific subcase in case3 where nr_items in root node is 3.
 */
int bplus_tree_delete_handle_case3_1(bplus_tree_balance_st *tb)
{

	int i, rc = EOK;
	int root_pos = 0;
	void *child_node0 = NULL;
	void *child_node1 = NULL;
	void *root_node = NULL;
	void *leaf_node = NULL;
	char *leaf_node_path = NULL;
	char *new_root_node_path = NULL;
	bplus_tree_traverse_path_st *traverse_path = NULL;
	block_head_st *block_head_child0, *block_head_child1, *block_head_root;
	block_head_st *block_head_leaf;
	uint16_t nr_keys_child0, nr_keys_child1;
	uint16_t nr_dc_child0, nr_dc_child1;
	uint16_t nr_items_child0, nr_items_child1;
	disk_child_st *dc = NULL;
	item_st *tmp_item = NULL;
	b_plus_tree_key_t key;
	ino_t new_root_ino;

	traverse_path = tb->tb_path;

	/*
	 * Lets figure out child0 and child1
	 */
	bplus_tre_get_child_0_1(tb, &child_node0, &child_node1, &new_root_node_path);

	root_node = bplus_tree_get_node_path(traverse_path, (BTREE_LEAF_LEVEL + 2));
	CHECK_RC_ASSERT((root_node == NULL), 0);
	CHECK_RC_ASSERT((child_node0 == NULL), 0);
	CHECK_RC_ASSERT((child_node1 == NULL), 0);

	/*
	 * Lets figure out whether both the children nodes could be merged.
	 */
	block_head_child0 = bplus_tree_get_block_head(child_node0);
	block_head_child1 = bplus_tree_get_block_head(child_node1);

	CHECK_RC_ASSERT((block_head_child0->level > BTREE_LEAF_LEVEL), 1);
	CHECK_RC_ASSERT((block_head_child1->level > BTREE_LEAF_LEVEL), 1);

	/*
	 * If child0 and child1 are underflown, then merge them. Otherwise return.
	 */
	nr_items_child0 = block_head_child0->nr_items;
	nr_items_child1 = block_head_child1->nr_items;
	if ((nr_items_child0 + nr_items_child1) >= (MAX_KEYS + MAX_DC))
	{
		return rc;
	}

	/*
	 * Read 0th dc of child1 and find out 0th item in this dc.
	 */
	leaf_node_path = (char *)malloc(MAX_PATH);
	leaf_node = (void *)malloc(NODE_SIZE);
	CHECK_RC_ASSERT((leaf_node_path == NULL), 0);
	CHECK_RC_ASSERT((leaf_node == NULL), 0);

	dc = bplus_tree_get_dc(child_node1, 0);
	CHECK_RC_ASSERT((dc->i_ino > 0), 1);
	rc = get_path(META_DIR, dc->i_ino, leaf_node_path);
	CHECK_RC_ASSERT(rc, EOK);
	rc = read_file_contents(leaf_node_path,
				leaf_node,
				READ_FLAGS,
				READ_MODE,
				NODE_SIZE);
	CHECK_RC_ASSERT(rc, EOK);
	block_head_leaf = bplus_tree_get_block_head(leaf_node);
	CHECK_RC_ASSERT((block_head_leaf->level), BTREE_LEAF_LEVEL);
	tmp_item = bplus_tree_get_item(leaf_node, 0);
	CHECK_RC_ASSERT((tmp_item->i_ino > 0), 1);

	/*
	 * Lets mark the root node for deletion.
	 */
	block_head_root = bplus_tree_get_block_head(root_node);
	bplus_tree_mark_for_delete(block_head_root);

	nr_keys_child0 = NR_KEYS(nr_items_child0);
	nr_keys_child1 = NR_KEYS(nr_items_child1);
	nr_dc_child0 = nr_keys_child0 + 1;
	nr_dc_child1 = nr_keys_child1 + 1;

	/*
	 * Punch the key of the item in child_node0 at proper location.
	 */
	memset(&key, 0, KEY_SIZE);
	memcpy(&(key.i_ino), &(tmp_item->i_ino), sizeof(ino_t));
	bplus_tree_punch_key(child_node0, &key, nr_keys_child0);
	nr_keys_child0++;

	/*
	 * Lets flow all the keys and dc in one single node.
	 */
	for (i = 0; i < nr_keys_child1; i++)
	{

		rc = bplus_tree_flow_key(child_node1,
					 child_node0,
					 (i + 1),
					 nr_keys_child0);
		CHECK_RC_ASSERT(rc, EOK);
		nr_keys_child0++;

	}

	for (i = 0; i < nr_dc_child1; i++)
	{

		rc = bplus_tree_flow_dc(child_node1,
					child_node0,
					i,
					nr_dc_child0);
		CHECK_RC_ASSERT(rc, EOK);
		nr_dc_child0++;

	}

	/*
	 * Now, adjust new root.
	 */
	rc = is_path_present(new_root_node_path, &new_root_ino);
	CHECK_RC_ASSERT(rc, EOK);
	bplus_tree_adjust_root(tb->tb_root_path, new_root_ino);

	free(leaf_node_path);
	free(leaf_node);
	return rc;

}

/*
 * This function handles a specific case in deletion where height of B+ tree is 2 or
 * greater.
 */
int bplus_tree_delete_handle_case3(bplus_tree_balance_st *tb)
{

	int rc = EOK;
	void *root_node = NULL;
	bplus_tree_traverse_path_st *traverse_path = NULL;
	block_head_st *block_head_root = NULL;
	uint16_t nr_items_root = 0;
	int root_pos = 0;

	traverse_path = tb->tb_path;

	CHECK_RC_ASSERT((traverse_path == NULL), 0);
	CHECK_RC_ASSERT((traverse_path->path_length >= 2), 1);

	root_node = bplus_tree_get_node_path(traverse_path, (BTREE_LEAF_LEVEL + 2));
	CHECK_RC_ASSERT((root_node == NULL), 0);

	block_head_root = bplus_tree_get_block_head(root_node);
	nr_items_root = block_head_root->nr_items;
	CHECK_RC_ASSERT((nr_items_root >= 3), 1);

	if (nr_items_root == 3)
	{
		rc = bplus_tree_delete_handle_case3_1(tb);
	}

	return rc;

}

/*
 * This function is core deletion logic and handles every case of deletion.
 */
int bplus_tree_rebalance_delete_handle(bplus_tree_balance_st *tb)
{

	int rc = EOK;
	bplus_tree_traverse_path_st *traverse_path = NULL;
	int nr_right_siblings = 0;

	traverse_path = tb->tb_path;
	CHECK_RC_ASSERT((traverse_path == NULL), 0);

	/*
	 * Single node B+ tree should not come here.
	 */
	CHECK_RC_ASSERT((traverse_path->path_length > 0), 1);

	/*
	 * Figure out whether right sibling for the node is present or not.
	 */
	nr_right_siblings = tb->tb_nr_right[BTREE_LEAF_LEVEL];
	if (nr_right_siblings == 0)
	{
		rc = bplus_tree_delete_handle_case1(tb);
	}
	else
	{
		rc = bplus_tree_delete_handle_case2(tb);
	}

	/*
	 * If height of B+ tree is 2 or greater, then handle this scenario.
	 */
	if (traverse_path->path_length >= 2)
	{
		rc = bplus_tree_delete_handle_case3(tb);
	}

	return rc;

}

/*
 * This function does rebalancing of b+ tree while deletion.
 */
int bplus_tree_rebalance_delete(char *root_path,
				bplus_tree_traverse_path_st *traverse_path)
{

	int i, rc = EOK;
	bplus_tree_balance_st *tb;

	tb = bplus_tree_init_tb(root_path, traverse_path);
	CHECK_RC_ASSERT((tb == NULL), 0);

	rc = bplus_tree_rebalance_delete_handle(tb);
	if (rc != EOK)
	{
	
		bplus_tree_deinit_tb(tb);
		return rc;

	}

	rc = bplus_tree_flush_tb(tb);
	if (rc != EOK)
	{

		bplus_tree_deinit_tb(tb);
		return rc;

	}

	bplus_tree_deinit_tb(tb);

	return rc;

}

