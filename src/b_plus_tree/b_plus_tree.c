#include "b_plus_tree_interface.h"

/*
 * This function writes inode no. of root node into file pointed by root_path.
 */
int bplus_tree_write_root_path(char *root_path, ino_t root_ino)
{

	int rc = EOK;

	CHECK_RC_ASSERT((root_path == NULL), 0);

	rc = write_file_contents(root_path,
				 OPEN_FLAGS,
				 WRITE_MODE,
				 &root_ino,
				 sizeof(ino_t));
	CHECK_RC_ASSERT(rc, EOK);
	return rc;

}

/*
 * This function initializes b+ tree subsystem which involves following :
 * 1. Create meta directory. This directory stores all the internal and leaf nodes.
 * 2. Create and format root node of b+ tree.
 * 3. If meta directory is already present, then don't create it. I believe it is a
 *    request to reuse the existing b+ tree.
 * 4. Pass force_init flag, which indicates to delete and to create meta directory.
 * 5. Format meaning write a block_head data structure.
 */
int bplus_tree_init(char *meta_dir, char *root_path, bool force_init)
{

	int rc = EOK, rc1;
	ino_t i_ino, meta_i_ino;
	char *path;

	CHECK_RC_ASSERT((meta_dir == NULL), 0);
	CHECK_RC_ASSERT((root_path == NULL), 0);

	rc = is_path_present(meta_dir, &meta_i_ino);

	/*
	 * If stat yields success and force_init is true, then delete the .meta.
	 */
	if (rc == EOK)
	{

		if (force_init == TRUE)
		{

			rc1 = bplus_tree_deinit(meta_dir);
			if (rc1 != EOK)
			{
				return rc1;
			}
			rc = ENOENT;

		}
		else
		{
			return rc;
		}

	}
	CHECK_RC_ASSERT(rc, ENOENT);

	/*
	 * By this time, .meta shouldn't be present. Now, create a meta directory.
	 */
	rc = mkdir(meta_dir, 0744);
	CHECK_RC_ASSERT(rc, EOK);

	/*
	 * Now, allocate a root node of the B+ tree under .meta.
	 */
	rc = bplus_tree_allocate_node_num(&i_ino);
	CHECK_RC_ASSERT(rc, EOK);

	path = (char *)malloc(MAX_PATH);
	CHECK_RC_ASSERT((path == NULL), 0);
	memset(path, 0, MAX_PATH);
	snprintf(path, MAX_PATH, "%s/%d", META_DIR, (int)i_ino);

	/*
	 * Now, format the root node pointed by path. We will get inode no. of root.
	 */
	rc = bplus_tree_allocate_node(path, &i_ino);
	CHECK_RC_ASSERT(rc, EOK);

	/*
	 * Write the inode no. in to root file.
	 */
	rc = bplus_tree_write_root_path(root_path, i_ino);
	CHECK_RC_ASSERT(rc, EOK);

	free(path);
	return rc;

}

/*
 * This function removes meta directory.
 */
int bplus_tree_deinit(char *meta_dir)
{

	int rc = EOK;
	char cmd[20] = {0};

	CHECK_RC_ASSERT((meta_dir == NULL), 0);
	snprintf(cmd, 20, "rm -rf %s", meta_dir);
	rc = system(cmd);
	CHECK_RC_ASSERT(rc, EOK);
	return rc;

}

/*
 * This function does multiple things : 
 * 1. Creates a file at path.
 * 2. Does a stat on the file and returns inode number.
 * 3. Formats a file to write block_head data structure into it.
 */
int bplus_tree_allocate_node(char *path, ino_t *i_ino)
{

	int rc = EOK;
	struct stat *stat_buf = NULL;

	CHECK_RC_ASSERT((i_ino == NULL), 0);
	CHECK_RC_ASSERT((path == NULL), 0);

	rc = bplus_tree_format_node(path);
	CHECK_RC_ASSERT(rc, EOK);

	stat_buf = (struct stat*)malloc(sizeof(struct stat));
	CHECK_RC_ASSERT((stat_buf == NULL), 0);
	memset(stat_buf, 0, sizeof(struct stat));

	*i_ino = 0;
	if (stat(path, stat_buf) == -1)
	{

		rc = errno;
		free(stat_buf);
		return rc;

	}

	*i_ino = stat_buf->st_ino;
	free(stat_buf);

	return rc;

}

/*
 * This function does multiple things as follow :
 * 1. It opens file to truncate the contents.
 * 2. It writes out block_head with default values inside file.
 */
int bplus_tree_format_node(char *path)
{

	int rc = EOK;
	block_head_st *block_head = NULL;
	void *node;

	/*
	 * Allocate whole node of NODE_SIZE and write it on disk.
	 */
	node = (void *)malloc(NODE_SIZE);
	CHECK_RC_ASSERT((node == NULL), 0);
	memset(node, 0, NODE_SIZE);

	block_head = node;
	block_head->nr_items = 0;
	block_head->free_space = NODE_SIZE - BLOCK_HEAD_SIZE;
	block_head->level = BTREE_LEAF_LEVEL;
	rc = write_file_contents(path, OPEN_FLAGS, OPEN_MODE, node, NODE_SIZE);

	free(node);
	return rc;

}

/*
 * This function does following :
 * 1. Calculate key based on path.
 * 2. Search the key in the b+ tree rooted at root_path.
 * 3. If key found in the b+ tree, delete the key.
 */
int bplus_tree_delete(char *root_path, char *path)
{

	int rc = EOK;
	b_plus_tree_key_t *key = NULL;
	bplus_tree_traverse_path_st *traverse_path = NULL;
	item_st *item = NULL;

	CHECK_RC_ASSERT((root_path == NULL), 0);
	CHECK_RC_ASSERT((path == NULL), 0);

	key = (b_plus_tree_key_t*)malloc(KEY_SIZE);
	CHECK_RC_ASSERT((key == NULL), 0);

	rc = bplus_form_key(path, key);
	if (rc != EOK)
	{

		free(key);
		return rc;
	}

	traverse_path = (bplus_tree_traverse_path_st *)malloc(TRAVERSE_PATH_SIZE);
	CHECK_RC_ASSERT((traverse_path == NULL), 0);
	memset(traverse_path, 0, sizeof(bplus_tree_traverse_path_st));
	traverse_path->path_length = INVALID_PATH_LENGTH;

	rc = bplus_tree_search_key(root_path, key, traverse_path);
	if (rc == ENOENT)
	{

		free(key);
		bplus_tree_free_traverse_path(traverse_path);
		return rc;

	}

	rc = bplus_tree_delete_item(root_path, traverse_path);
	if (rc != EOK)
	{

		free(key);
		bplus_tree_free_traverse_path(traverse_path);
		return rc;

	}

	rc = bplus_tree_flush_traverse_path(traverse_path);
	if (rc != EOK)
	{

		free(key);
		bplus_tree_free_traverse_path(traverse_path);
		return rc;

	}

	free(key);
	bplus_tree_free_traverse_path(traverse_path);
	return rc;

}

/*
 * This function does following : 
 * 1. Calculate unique key based on the path.
 * 2. Search the key into b+ tree rooted at root_path.
 * 3. Insert the key and inode no. of the object pointed by path in b+ tree.
 */
int bplus_tree_insert(char *root_path, char *path)
{

	int rc = EOK;
	b_plus_tree_key_t *key = NULL;
	bplus_tree_traverse_path_st *traverse_path = NULL;
	item_st *item = NULL;

	CHECK_RC_ASSERT((root_path == NULL), 0);
	CHECK_RC_ASSERT((path == NULL), 0);

	key = (b_plus_tree_key_t*)malloc(KEY_SIZE);
	CHECK_RC_ASSERT((key == NULL), 0);

	rc = bplus_form_key(path, key);
	if (rc != EOK)
	{

		free(key);
		return rc;

	}

	traverse_path = (bplus_tree_traverse_path_st *)malloc(TRAVERSE_PATH_SIZE);
	CHECK_RC_ASSERT((traverse_path == NULL), 0);
	memset(traverse_path, 0, sizeof(bplus_tree_traverse_path_st));
	traverse_path->path_length = INVALID_PATH_LENGTH;

	rc = bplus_tree_search_key(root_path, key, traverse_path);
	if (rc == EEXIST)
	{

		free(key);
		bplus_tree_free_traverse_path(traverse_path);
		return rc;

	}

	item = (item_st *)malloc(ITEM_SIZE);
	CHECK_RC_ASSERT((item == NULL), 0);
	bplus_tree_form_item(key, item);

	rc = bplus_tree_insert_item(root_path, traverse_path, item);
	if (rc != EOK)
	{

		free(item);
		free(key);
		bplus_tree_free_traverse_path(traverse_path);
		return rc;

	}

	rc = bplus_tree_flush_traverse_path(traverse_path);
	if (rc != EOK)
	{

		free(item);
		free(key);
		bplus_tree_free_traverse_path(traverse_path);
		return rc;

	}

	free(item);
	free(key);
	bplus_tree_free_traverse_path(traverse_path);
	return rc;

}

/*
 * This function does following :
 * 1. Does stat on path.
 * 2. Copies st_ino into key->i_ino. 
 */
int bplus_form_key(char *path, b_plus_tree_key_t *key)
{

	int rc = EOK;
	
	CHECK_RC_ASSERT((path == NULL), 0);
	CHECK_RC_ASSERT((key == NULL), 0);

	memset(key, 0, KEY_SIZE);
	rc = is_path_present(path, &(key->i_ino));
	if (rc != EOK)
	{
		return rc;
	}

	return rc;

}

/*
 * This function does following : 
 * 1. Search key in the b+ tree rooted at root_path.
 * 2. Fill up the traverse path as we go down.
 * Return values from this function are : 
 * 1. EEXIST if key is found in the b+ tree.
 * 2. ENOENT if key is not found the b+ tree.
 */
int bplus_tree_search_key(char *root_path,
			  b_plus_tree_key_t *key,
			  bplus_tree_traverse_path_st *traverse_path)
{

	bool is_leaf = FALSE;
	disk_child_st *dc = NULL;
	int rc = EEXIST;
	char *path = root_path;
	char *next_path = NULL;
	void *buf = NULL;
	block_head_st *block_head = NULL;
	uint8_t level;
	int position = -1;
	int len, item_len;
	ino_t root_ino;

	CHECK_RC_ASSERT((root_path == NULL), 0);
	CHECK_RC_ASSERT((key == NULL), 0);
	CHECK_RC_ASSERT((traverse_path == NULL), 0);

	buf = (void *)malloc(NODE_SIZE);
	CHECK_RC_ASSERT((buf == NULL), 0);

	next_path = (char *)malloc(MAX_PATH);
	CHECK_RC_ASSERT((next_path == NULL), 0);

	rc = read_file_contents(root_path,
				&root_ino,
				READ_FLAGS,
				READ_MODE,
				sizeof(ino_t));
	CHECK_RC_ASSERT(rc, EOK);

	rc = get_path(META_DIR, root_ino, next_path);
	if (rc != EOK)
	{

		free(next_path);
		free(buf);
		return rc;

	}

	path = next_path;
	traverse_path->path_length = INVALID_PATH_LENGTH;

	while (1)
	{

		/*
		 * Read file contents pointed by path first.
		 */
		rc = read_file_contents(path,
					buf,
					READ_FLAGS,
					READ_MODE,
					NODE_SIZE);
		if (rc != EOK)
		{
			break;
		}

		block_head = bplus_tree_get_block_head(buf);
		level = block_head->level;

		if (level == BTREE_LEAF_LEVEL)
		{
			is_leaf = TRUE;
		}

		rc = bin_search(buf, key, (block_head->nr_items - 1),
				&position, is_leaf);
		bplus_tree_copy_pe(traverse_path, buf, level, position, path);
		INCR_PATH_LENGTH(traverse_path);

		/*
		 * If we reach at the leaf block, break.
		 */
		if (is_leaf == TRUE)
		{
			break;
		}

		/*
		 * Now, following case cover for internal block.
		 */
		/*
		 * Don't break for internal node even if key exists.
		 * Let's go for next level down.
		 */

		dc = bplus_tree_get_dc(buf, position);
		rc = get_path(META_DIR, dc->i_ino, next_path);
		path = next_path;

	}
	
	free(buf);
	free(next_path);
	return rc;

}

/*
 * This function compares bplus tree keys.
 */
int bplus_key_compare(const void *key1, const void *key2)
{

	int rc = IDENTICAL;

	b_plus_tree_key_t *key_1 = (b_plus_tree_key_t *)key1;
	b_plus_tree_key_t *key_2 = (b_plus_tree_key_t *)key2;

	if (key_1->i_ino < key_2->i_ino)
	{
		rc = FIRST_LESS;
	}
	else if (key_1->i_ino > key_2->i_ino)
	{
		rc = FIRST_GREATER;
	}
	else
	{
		rc = IDENTICAL;
	}

	return rc;

}

/*
 * This is a generic binary search program.
 */
int bin_search(
	void *buf,
	b_plus_tree_key_t *key,
	int16_t nr_items,
	int *position,
	bool is_leaf)
{

	int rc = ENOENT;
	int start = 0;
	int end = nr_items;
	int i, mid;
	b_plus_tree_key_t key_to_compare;
	item_st *item;

	*position = 0;

	if (is_leaf == FALSE)
	{
		end = NR_KEYS(nr_items);
	}

	while (start <= end)
	{

		mid = (start + end) / (2);

		if (is_leaf)
		{

			item = bplus_tree_get_item(buf, mid);
			memcpy(&(key_to_compare.i_ino), &(item->i_ino), KEY_SIZE);

		}
		else
		{

			memcpy(&key_to_compare,
				bplus_tree_get_key(buf, mid),
				KEY_SIZE);

		}

		if (key_to_compare.i_ino > key->i_ino)
		{
			end = mid - 1;
		}
		else if (key_to_compare.i_ino < key->i_ino)
		{
			start = mid + 1;
		}
		else
		{

			rc = EEXIST;

			/*
			 * If it is internal node, right disk_child is located at 
			 * 1 position ahead of key.
			 */
			if (is_leaf == TRUE)
			{
				*position = mid;
			}
			else
			{
				*position = mid + 1;
			}
			break;

		}

	}

	if (rc != EEXIST)
	{
		*position = start;
	}

	return rc;

}

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

	rc = bplus_tree_rebalance_insert_handle(tb, item);
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

/*
 * This function rebalances internal node.
 */
int bplus_tree_rebalance_internal_insert(bplus_tree_balance_st *tb,
					 item_st *item, uint8_t level)
{

	int rc = EOK;

	return rc;

}

/*
 * This function handles internal nittie-gritties of rebalancing while insertion.
 */
int bplus_tree_rebalance_insert_handle(bplus_tree_balance_st *tb, item_st *item)
{

	int rc = EOK;
	int i;

	for (i = 0; i < BPLUS_MAX_HEIGHT; i++)
	{

		if (i == BTREE_LEAF_LEVEL)
		{
			rc = bplus_tree_rebalance_leaf_insert(tb, item);
		}
		else
		{
			rc = bplus_tree_rebalance_internal_insert(tb, item, i);
		}

	}

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

/*
 * This is driver function for rebalance of b+ tree.
 */
int bplus_tree_rebalance(char *root_path,
			 bplus_tree_traverse_path_st *traverse_path,
			 item_st *item,
			 rebalnce_mode_et mode)
{

	int rc = EOK;

	if (mode == BPLUS_INSERT)
	{
		rc = bplus_tree_rebalance_insert(root_path, traverse_path, item);
	}
	else
	{
		rc = bplus_tree_rebalance_delete(root_path, traverse_path);
	}

	return rc;

}

