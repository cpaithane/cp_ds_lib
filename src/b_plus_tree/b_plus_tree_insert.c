/*
 * This file contains all the APIs used for insertion of key-value pairs in B+ tree.
 */

#include "b_plus_tree_interface.h"

/*
 * This function inserts an item and re-balances b+ tree.
 */
int bplus_tree_insert_item(char *root_path,
			   bplus_tree_traverse_path_st *traverse_path, 
			   item_st *item)
{

	item_st *tmp_item = NULL;
	int i;
	int rc = EOK;
	void *leaf_node = NULL;
	block_head_st *block_head = NULL;

	leaf_node = traverse_path->path_elements[BTREE_LEAF_LEVEL].pe_node;
	CHECK_RC_ASSERT((leaf_node == NULL), 0);

	block_head = bplus_tree_get_block_head(leaf_node);

	/*
	 * Check if rebalancing of b+ tree is required.
	 */
	if (block_head->nr_items == MAX_ITEMS)
	{

		rc = bplus_tree_rebalance(root_path,
					  traverse_path,
					  item, BPLUS_INSERT);
		return rc;
	}

	/*
	 * Right shift one item.
	 */
	i = bplus_tree_shift_right(leaf_node, item, block_head->nr_items);

	/*
	 * Punch in item at location i.
	 */
	bplus_tree_punch_item(leaf_node, item, i);

	return rc;

}

/*
 * This function does rebalancing of b+ tree while insertion.
 */
int bplus_tree_rebalance_insert(char *root_path,
				bplus_tree_traverse_path_st *traverse_path,
				item_st *item)
{

	int i, rc = EOK;
	bplus_tree_balance_st *tb;

	tb = bplus_tree_init_tb(root_path, traverse_path);
	CHECK_RC_ASSERT((tb == NULL), 0);

	rc = bplus_tree_rebalance_leaf_insert(tb, item);
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

/*
 * This function rebalances leaf node.
 */
int bplus_tree_rebalance_leaf_insert(bplus_tree_balance_st *tb, item_st *item)
{

	int rc = EOK;
	int pe_position;
	void *leaf_node;
	bool flow;
	block_head_st *block_head;
	bplus_tree_traverse_path_st *traverse_path;
	ino_t leaf_ino;

	traverse_path = tb->tb_path;
	
	leaf_node = bplus_tree_get_node_path(traverse_path, BTREE_LEAF_LEVEL);
	CHECK_RC_ASSERT((leaf_node == NULL), 0);

	pe_position = bplus_tree_get_pos_path(traverse_path, BTREE_LEAF_LEVEL);
	block_head = bplus_tree_get_block_head(leaf_node);
	CHECK_RC_ASSERT(block_head->nr_items, MAX_ITEMS);

	flow = bplus_tree_find_flow_dir(pe_position);

	/*
	 * Allocate leaf node to accommodate the item.
	 */
	rc = bplus_tree_get_new_node(&leaf_ino);
	CHECK_RC_ASSERT(rc, EOK);

	rc = bplus_tree_flow_item_handle(tb, item, leaf_ino, flow);

	return rc;

}

