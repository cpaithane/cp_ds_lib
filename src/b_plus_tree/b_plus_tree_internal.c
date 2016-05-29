#include "b_plus_tree_interface.h"

/*
 * This function returns pointer to block_head_t.
 */
block_head_st *bplus_tree_get_block_head(void *buf)
{

	block_head_st *block_head = NULL;

	CHECK_RC_ASSERT((buf == NULL), 0);
	return ((block_head_st *)buf);

}

/*
 * This function copies every part of path_element.
 */
void bplus_tree_fill_pe(void *buf,
			char *path,
			int position,
			path_element_st *pe)
{

	void *node = NULL;
	char *pe_path = NULL;
	int len;

	memset(pe, 0, PE_SIZE);

	CHECK_RC_ASSERT((path == NULL), 0);

	node = (void *)malloc(NODE_SIZE);
	CHECK_RC_ASSERT((node == NULL), 0);

	/*
	 * strlen returns output excluding '\0'. Valgrind complains.
	 */
	len = strlen(path) + 1;
	pe_path = (char *)malloc(len);
	CHECK_RC_ASSERT((pe_path == NULL), 0);

	memset(node, 0, NODE_SIZE);
	memcpy(node, buf, NODE_SIZE);

	memset(pe_path, 0, len);
	memcpy(pe_path, path, len);
	pe_path[len - 1] = '\0';
	CHECK_RC_ASSERT((len - 1), strlen(pe_path));

	pe->pe_node = node;
	pe->pe_path = pe_path;
	pe->pe_position = position;

}

/*
 * This function copies buf at traverse path.
 */
void bplus_tree_copy_pe(bplus_tree_traverse_path_st *traverse_path,
			void *buf,
			uint8_t level,
			int position,
			char *path)
{

	path_element_st *pe;

	pe = (path_element_st *)malloc(PE_SIZE);
	CHECK_RC_ASSERT((pe == NULL), 0);
	bplus_tree_fill_pe(buf, path, position, pe);
 
	traverse_path->path_elements[level].pe_node = pe->pe_node;
	traverse_path->path_elements[level].pe_position = pe->pe_position;
	traverse_path->path_elements[level].pe_path = pe->pe_path;

	free(pe);

}

/*
 * This function gets disk child stored at position.
 */
disk_child_st *bplus_tree_get_dc(void *buf, int position)
{

	block_head_st *block_head = NULL;
	disk_child_st *dc = NULL;

	CHECK_RC_ASSERT((buf == NULL), 0);

	block_head = bplus_tree_get_block_head(buf);
	CHECK_RC_ASSERT((block_head->level == BTREE_LEAF_LEVEL), 0);

	dc = (disk_child_st*)(buf + BLOCK_HEAD_SIZE + 
			     (MAX_KEYS * KEY_SIZE) + 
			     (position * DC_SIZE));
	
	return dc;

}

/*
 * This function returns item head located at position from leaf node.
 */
item_st *bplus_tree_get_item(void *buf, uint16_t position)
{

	item_st *item = NULL;
	item = (item_st *)(buf + BLOCK_HEAD_SIZE + (ITEM_SIZE * position));
	return item;

}

/*
 * This function returns key located at position from internal node.
 */
b_plus_tree_key_t *bplus_tree_get_key(void *buf, uint16_t position)
{

	block_head_st *block_head;
	b_plus_tree_key_t *key;
	CHECK_RC_ASSERT((buf == NULL), 0);

	block_head = bplus_tree_get_block_head(buf);
	CHECK_RC_ASSERT((block_head->level == BTREE_LEAF_LEVEL), 0);

	key = (b_plus_tree_key_t *)(buf + BLOCK_HEAD_SIZE + (KEY_SIZE * position));
	return key;

}

/*
 * This function creates an item from key.
 */
void bplus_tree_form_item(b_plus_tree_key_t *key, item_st *item)
{

	CHECK_RC_ASSERT((item == NULL), 0);
	memset(item, 0, ITEM_SIZE);
	memcpy(&(item->i_ino), &(key->i_ino), sizeof(ino_t));

}

/*
 * This function punches key inside internal node at position.
 */
int bplus_tree_punch_key(void *buf, b_plus_tree_key_t *key, uint16_t position)
{

	int rc = EOK;
	b_plus_tree_key_t *key_pos;
	block_head_st *block_head;

	key_pos = bplus_tree_get_key(buf, position);
	CHECK_RC_ASSERT((key_pos == NULL), 0);
	memcpy(key_pos, key, KEY_SIZE);

	/*
	 * Adjust counters
	 */
	block_head = bplus_tree_get_block_head(buf);
	block_head->nr_items += 1;
	block_head->free_space -= KEY_SIZE;

	return rc;

}

/*
 * This function punches disk-child inside internal node at position.
 */
int bplus_tree_punch_dc(void *buf, disk_child_st *dc, uint16_t position)
{

	int rc = EOK;
	disk_child_st *dc_pos;
	block_head_st *block_head;

	dc_pos = bplus_tree_get_dc(buf, position);
	CHECK_RC_ASSERT((dc_pos == NULL), 0);
	memcpy(dc_pos, dc, DC_SIZE);

	/*
	 * Adjust counters
	 */
	block_head = bplus_tree_get_block_head(buf);
	block_head->nr_items += 1;
	block_head->free_space -= DC_SIZE;
	return rc;

}

/*
 * This function punches item inside leaf node at position.
 */
int bplus_tree_punch_item(void *buf, item_st *item, uint16_t position)
{

	int rc = EOK;
	item_st *item_pos;
	block_head_st *block_head;

	item_pos = bplus_tree_get_item(buf, position);
	CHECK_RC_ASSERT((item_pos == NULL), 0);
	memcpy(item_pos, item, ITEM_SIZE);

	/*
	 * Adjust counters
	 */
	block_head = bplus_tree_get_block_head(buf);
	block_head->nr_items += 1;
	block_head->free_space -= ITEM_SIZE;
	return rc;

}

/*
 * This function flushes all siblings pointed by head of list.
 */
int bplus_tree_flush_siblings(sll_st *tb_left)
{

	int rc = EOK;
	void *node;
	char *path;
	path_element_st *path_element;

	while (tb_left)
	{

		path_element = (path_element_st *)tb_left->sll_data;
		node = path_element->pe_node;
		path = path_element->pe_path;
		rc = write_file_contents(path, WRITE_FLAGS, WRITE_MODE,
					 node, NODE_SIZE);
		CHECK_RC_ASSERT(rc, EOK);

		/*
		 * fill_pe allocates node and path. Lets deallocate them.
		 */
		free(node);
		free(path);
		tb_left = tb_left->sll_next;

	}

	return rc;

}

/*
 * This function flushes tree balance nodes.
 */
int bplus_tree_flush_tb(bplus_tree_balance_st *tb)
{

	int i, rc = EOK;
	int nr_left_nodes, nr_right_nodes;
	sll_st *tb_left, *tb_right;

	/*
	 * traverse_path will be flushed by caller.
	 */

	/*
	 * Flush left and right neighbors. 
	 */
	for (i = 0; i < BPLUS_MAX_HEIGHT; i++)
	{

		tb_left = tb->tb_left[i];
		tb_right = tb->tb_right[i];
		bplus_tree_flush_siblings(tb_left);
		bplus_tree_flush_siblings(tb_right);
		
	}
	
	return rc;

}

/*
 * This function flushes all in-memory buffers to disk.
 */
int bplus_tree_flush_traverse_path(bplus_tree_traverse_path_st *traverse_path)
{

	int i;
	char *path;
	void *node;
	int rc = EOK;

	for (i= 0; i < BPLUS_MAX_HEIGHT; i++)
	{

		if (traverse_path->path_elements[i].pe_node == NULL)
		{
			continue;
		}

		path = traverse_path->path_elements[i].pe_path;
		node = traverse_path->path_elements[i].pe_node;
		rc = write_file_contents(path, WRITE_FLAGS, WRITE_MODE,
					 node, NODE_SIZE);
		if (rc != EOK)
		{
			break;
		}

	}

	return rc;

}

/*
 * This function frees memory allocate to traverse_path.
 */
int bplus_tree_free_traverse_path(bplus_tree_traverse_path_st *traverse_path)
{

	int rc = EOK;
	int i;
	char *path;
	void *node;
	
	for (i= 0; i < BPLUS_MAX_HEIGHT; i++)
	{

		if (traverse_path->path_elements[i].pe_node == NULL)
		{
			continue;
		}

		path = traverse_path->path_elements[i].pe_path;
		node = traverse_path->path_elements[i].pe_node;

		free(path);
		free(node);

	}

	free(traverse_path);
	return rc;

}

/*
 * This function frees up path_element and members inside it.
 */
int bplus_tree_free_pe(path_element_st *path_element)
{

	free(path_element->pe_path);
	free(path_element->pe_node);
	free(path_element);

}

/*
 * This function left shifts one item from leaf node. Returns the slot where to 
 * insert/delete new item
 */
int bplus_tree_shift_left(void *leaf_node, int position, uint16_t nr_items)
{

	int i;
	item_st *tmp_item1, *tmp_item2;

	for (i = position; i < (nr_items - 1); i++)
	{

		tmp_item1 = bplus_tree_get_item(leaf_node, i);
		tmp_item2 = bplus_tree_get_item(leaf_node, i + 1);
		memcpy(tmp_item1, tmp_item2, ITEM_SIZE);

	}

	return i;

}

/*
 * This function right shifts disk children to make space at position.
 */
int bplus_tree_shift_right_dc(void *internal_node, int position, uint16_t nr_dc)
{

	disk_child_st *dc1, *dc2;
	int j;

	for (j = nr_dc; j > position; j--)
	{

		dc1 = bplus_tree_get_dc(internal_node, j - 1);
		dc2 = bplus_tree_get_dc(internal_node, j);
		memcpy(dc2, dc1, DC_SIZE);

	}

	return j;

}

/*
 * This function shifts keys to right to find out right position for key.
 */
int bplus_tree_shift_right_keys(void *internal_node,
				b_plus_tree_key_t *key,
				uint16_t nr_keys)
{

	int i, j;
	b_plus_tree_key_t *tmp_key = NULL;
	b_plus_tree_key_t *tmp_key1 = NULL;
	b_plus_tree_key_t *tmp_key2 = NULL;

	for (i = 0; i < nr_keys; i++)
	{

		tmp_key = bplus_tree_get_key(internal_node, i);
		if (key->i_ino < tmp_key->i_ino)
		{

			for (j = nr_keys; j > i; j--)
			{

				tmp_key1 = bplus_tree_get_key(internal_node, j - 1);
				tmp_key2 = bplus_tree_get_key(internal_node, j);
				memcpy(tmp_key2, tmp_key1, KEY_SIZE);
				memset(tmp_key1, 0x0, KEY_SIZE);

			}

			break;

		}

	}

	return i;

}

/*
 * This function right shifts one item from leaf node. Returns the slot where to 
 * insert new item
 */
int bplus_tree_shift_right(void *leaf_node, item_st *item, uint16_t nr_items)
{

	int i, j;
	item_st *tmp_item = NULL;
	item_st *tmp_item1 = NULL;
	item_st *tmp_item2 = NULL;

	for (i = 0; i < nr_items; i++)
	{

		tmp_item = bplus_tree_get_item(leaf_node, i);
		if (item->i_ino < tmp_item->i_ino)
		{

			for (j = nr_items; j > i; j--)
			{

				tmp_item1 = bplus_tree_get_item(leaf_node, j - 1);
				tmp_item2 = bplus_tree_get_item(leaf_node, j);
				memcpy(tmp_item2, tmp_item1, ITEM_SIZE);

			}

			break;

		}

	}

	return i;

}

/*
 * This function gets node from traverse_path.
 */
void *bplus_tree_get_node_path(
		bplus_tree_traverse_path_st *traverse_path,
		uint8_t level)
{

	CHECK_RC_ASSERT((traverse_path == NULL), 0);
	return (traverse_path->path_elements[level].pe_node);

}

/*
 * This function gets parent node from traverse_path of level.
 */
void *bplus_tree_get_parent_node_path(
		bplus_tree_traverse_path_st *traverse_path,
		uint8_t level)
{

	CHECK_RC_ASSERT((traverse_path == NULL), 0);
	return (traverse_path->path_elements[level + 1].pe_node);

}

/*
 * This function gets position of a key from leaf node.
 */
int bplus_tree_get_pos_path(bplus_tree_traverse_path_st *traverse_path,
			    uint8_t level)
{

	CHECK_RC_ASSERT((traverse_path == NULL), 0);
	return (traverse_path->path_elements[level].pe_position);

}

/*
 * This function gets pe_path of a node at level.
 */
char *bplus_tree_get_pe_path_path(bplus_tree_traverse_path_st *traverse_path,
                            uint8_t level)
{

	CHECK_RC_ASSERT((traverse_path == NULL), 0);
	return (traverse_path->path_elements[level].pe_path);

}

/*
 * This function writes key and two disk children in an internal node.
 * This should be called where no item is added into internal node.
 */
void bplus_tree_init_internal_node(void *new_internal_node,
				   b_plus_tree_key_t *key,
				   disk_child_st *dc0,
				   disk_child_st *dc1)
{

	block_head_st *block_head;
	CHECK_RC_ASSERT((new_internal_node == NULL), 0);
	CHECK_RC_ASSERT((key == NULL), 0);
	CHECK_RC_ASSERT((dc0 == NULL), 0);
	CHECK_RC_ASSERT((dc1 == NULL), 0);

	block_head = bplus_tree_get_block_head(new_internal_node);
	CHECK_RC_ASSERT(block_head->nr_items, 0);
	CHECK_RC_ASSERT((block_head->level == BTREE_LEAF_LEVEL), 0);
	bplus_tree_punch_key(new_internal_node, key, 0);
	bplus_tree_punch_dc(new_internal_node, dc0, 0);
	bplus_tree_punch_dc(new_internal_node, dc1, 1);

}

/*
 * This function figures out the key to insert into parent node.
 */
void bplus_tree_get_key_to_insert(void *node,
				  b_plus_tree_key_t *key)
{

	item_st *first_item;
	first_item = bplus_tree_get_item(node, 0);
	key->i_ino = first_item->i_ino;

}

/*
 * This function inserts key and disk children into internal node.
 */
void bplus_tree_handle_internal_insert(
			bplus_tree_balance_st *tb,
			void *internal_node,
			char *internal_node_path,
			bool flow_mode,
			ino_t new_child_ino,
			ino_t internal_ino,
			b_plus_tree_key_t *key,
			uint16_t nr_keys_to_shift,
			uint16_t nr_dc_to_shift,
			uint8_t level)
{

	int rc = EOK;
	bplus_tree_traverse_path_st *traverse_path = tb->tb_path;
	disk_child_st dc, dc0, dc1;
	block_head_st *block_head;
	char *tmp_path;
	ino_t i_ino;

	tmp_path = bplus_tree_get_pe_path_path(
				traverse_path,
				level);
	rc = is_path_present(tmp_path, &i_ino);
	CHECK_RC_ASSERT(rc, EOK);

	dc0.i_ino = i_ino;
	dc1.i_ino = new_child_ino;

	/*
	 * Both the disk children should not point to same inode no.
	 */
	CHECK_RC_ASSERT((i_ino == new_child_ino), 0);

	/*
	 * If internal node is empty, then punch in key and 2 disk children
	 * Also, punch in the newly written internal_node in traverse_path.
	 *
	 * Now, root is changed. New root is internal_node here. Previously, root 
	 * was leaf_node. So, exchage the contents between leaf_node and internal 
	 * node.
	 * Adjust internal node's level.
	 */
	block_head = bplus_tree_get_block_head(internal_node);

	if (block_head->nr_items == 0)
	{

		block_head->level = (tb->tb_path->path_length + 1);
		bplus_tree_init_internal_node(internal_node,
					      key, &dc0, &dc1);
		rc = bplus_tree_adjust_root(tb->tb_root_path, internal_ino);
		INCR_PATH_LENGTH(tb->tb_path);
		bplus_tree_copy_pe(tb->tb_path,
				   internal_node,
				   block_head->level,
				   0, internal_node_path);

	}
	else
	{

		/*
		 * Copy appropriate disk child as per the flow_mode.
		 */
		memcpy(&dc, &dc1, DC_SIZE);
		bplus_tree_adjust_internal(internal_node,
					   nr_keys_to_shift,
					   nr_dc_to_shift,
					   key, &dc);

	}

	CHECK_RC_ASSERT((block_head->level > BTREE_LEAF_LEVEL), 1);

}

/*
 * This function does following things :
 * 1. Flows nr_items_to_flow from leaf_node to new_leaf_node.
 * 2. Punches the item into correct leaf node.
 */
void bplus_tree_adjust_leaf(void *leaf_node,
			    void *new_leaf_node,
			    item_st *item)
{

	int i;
	block_head_st *block_head;
	item_st *last_item;
	void *node;
	int nr_items_to_flow = (MAX_ITEMS / 2);

	for (i = 0; i < nr_items_to_flow; i++)
	{
		bplus_tree_flow_item(leaf_node, new_leaf_node);
	}

	/*
	 * Decide where to insert new item. There can be cases as follow :
	 * leaf_node = {2, 3}, new_leaf_node = {5, 7}.
	 *
	 * A. Item should be inserted in leaf_node in following case
	 * item = 4
	 *
 	 * B. Item should be inserted in new_leaf_node in following case
	 * item = 6
	 */
	block_head = bplus_tree_get_block_head(leaf_node);
	last_item = bplus_tree_get_item(leaf_node, (block_head->nr_items - 1));

	/*
	 * node below is node into which the item should ideally fit in.
	 */
	node = new_leaf_node;
	if (last_item->i_ino > item->i_ino)
	{
		node = leaf_node;
	}

	/*
	 * Now, shift right the node, punch an item and adjust counters.
	 */
	block_head = bplus_tree_get_block_head(node);
	i = bplus_tree_shift_right(node, item, block_head->nr_items);
	bplus_tree_punch_item(node, item, i);

}

/*
 * This function does following things : 
 * 1. Right shift keys to accommodate key
 * 2. Right shift disk children to accomodate disk child
 * 3. If right position to insert key is found to be 0, disk child will be inserted
 * at 0 position.
 * 4. Else, disk child will be inserted at (i+1) position.
 */
void bplus_tree_adjust_internal(void *internal_node,
				uint16_t nr_keys_to_shift,
				uint16_t nr_dc_to_shift,
				b_plus_tree_key_t *key,
				disk_child_st *dc)
{

	int i;
	block_head_st *block_head;
	int nr_keys, nr_dc;

	block_head = bplus_tree_get_block_head(internal_node);
	CHECK_RC_ASSERT((block_head->nr_items >= 3), 1);
	CHECK_RC_ASSERT((block_head->level == BTREE_LEAF_LEVEL), 0);

	/*
	 * Right shift keys to find right position for key.
	 */
	nr_keys = nr_keys_to_shift;
	nr_dc = nr_dc_to_shift;
	i = bplus_tree_shift_right_keys(internal_node, key, nr_keys);

	/*
	 * Punch the key at position i.
	 */
	bplus_tree_punch_key(internal_node, key, i);

	/*
	 * Now, right shift disk children. Accurate position for disk child is i + 1
	 */
	i = bplus_tree_shift_right_dc(internal_node, i + 1, nr_dc);

	/*
	 * Now, punch disk child at position i.
	 */
	bplus_tree_punch_dc(internal_node, dc, i);

}

/*
 * This function attaches left or right neighbor in a linked list maintained at 
 * level.
 */
void bplus_tree_attach_neighbor(bplus_tree_balance_st *tb,
				void *new_node,
				char *new_node_path,
				bool flow_mode,
				uint8_t level,
				int position)
{

	path_element_st *path_element;

	path_element = (path_element_st *)malloc(PE_SIZE);
	CHECK_RC_ASSERT((path_element == NULL), 0);
	bplus_tree_fill_pe(new_node, new_node_path, position, path_element);

	if (flow_mode == LEFT)
	{

		tb->tb_left[level] = 
			sll_insert_node_pos(
				tb->tb_left[level],
				path_element,
				tb->tb_nr_left[level],
				PE_SIZE);
		(tb->tb_nr_left[level])++;

	}
	else
	{

		tb->tb_right[level] = 
			sll_insert_node_pos(
				tb->tb_right[level],
				path_element,
				tb->tb_nr_right[level],
				PE_SIZE);
		(tb->tb_nr_right[level])++;

	}

	free(path_element);

}

/*
 * This is case #1 : 
 * This is a case where left and right neighbour is not present. That means, it is 
 * a single node b+ tree where root is full and new insertion request has 
 * arrived.
 * In this case, new_leaf_ino should be left/right neighbour of the node. So, 
 * (MAX_ITEMS / 2) items from the node will be flown to new_leaf_ino.
 */
int bplus_tree_flow_item_case1(
			    bplus_tree_balance_st *tb,
			    bool flow_mode,
			    void *leaf_node,
			    void *new_leaf_node,
			    void *internal_node,
			    item_st *item,
			    char *internal_node_path,
			    char *new_leaf_node_path,
			    ino_t new_leaf_ino,
			    ino_t internal_ino)
{

	int rc = EOK;
	b_plus_tree_key_t key;
	block_head_st *block_head;
	uint16_t nr_keys_to_shift, nr_dc_to_shift;

	/*
	 * First adjust leaf and new_leaf according to item.
	 */
	bplus_tree_adjust_leaf(leaf_node, new_leaf_node, item);

	/*
	 * Now, we have to keep B+ tree property intact as follow :
	 * 1. Copy first key into internal node.
	 */
	bplus_tree_get_key_to_insert(new_leaf_node, &key);

	/*
	 * 2. Also, disk_children needs to be formed in internal node. As child node
	 * is at leaf level, shifting happens at the leaf level. NR_ITEMS can be 
	 * safely used here.
	 */
	block_head = bplus_tree_get_block_head(internal_node);
	nr_keys_to_shift = NR_KEYS(block_head->nr_items);
	nr_dc_to_shift = nr_keys_to_shift + 1;
	bplus_tree_handle_internal_insert(tb,
					  internal_node,
					  internal_node_path,
					  flow_mode,
					  new_leaf_ino,
					  internal_ino,
					  &key,
					  nr_keys_to_shift,
					  nr_dc_to_shift,
					  BTREE_LEAF_LEVEL);

	/*
	 * 3. Attach the leaf node to tb and flush.
	 */
	bplus_tree_attach_neighbor(tb,
				   new_leaf_node,
				   new_leaf_node_path,
				   flow_mode,
				   BTREE_LEAF_LEVEL, 0);

	return rc;

}

/*
 * This function handles case 2 of item insertion into B+ tree. It does following:
 * 1. Do normal insertion into leaf node and adjust new leaf node.
 */
int bplus_tree_flow_item_case2(
			    bplus_tree_balance_st *tb,
			    bool flow_mode,
			    void *leaf_node,
			    void *new_leaf_node,
			    void *internal_node,
			    item_st *item,
			    char *internal_node_path,
			    char *new_leaf_node_path,
			    ino_t new_leaf_ino,
			    ino_t internal_ino)
{

	int i, rc = EOK;
	b_plus_tree_key_t key, *key1, *key2;
	ino_t new_internal_ino, root_internal_ino;
	char *tmp_internal_path, *new_internal_node_path, *root_node_path;
	block_head_st *block_head, *block_head_new;
	void *root_internal_node, *new_internal_node, *internal_node_to_insert;
	uint16_t nr_keys_to_flow, nr_dc_to_flow;
	uint16_t nr_keys_to_shift, nr_dc_to_shift;
	uint8_t level;
	uint16_t src_dc_pos, dst_dc_pos, nr_keys_internal, nr_dc_internal;
	uint16_t pos_to_reset;

	/*
	 * First adjust leaf and new_leaf according to item.
	 */
	bplus_tree_adjust_leaf(leaf_node, new_leaf_node, item);

	/*
	 * Now, we have to keep B+ tree property intact as follow :
	 * 1. Copy first key into internal node.
	 */
	bplus_tree_get_key_to_insert(new_leaf_node, &key);

	/*
	 * 2. As the parent of leaf node is (internal_node) full, we need to 
	 * allocate 2 new internal nodes. One node for a neighbor and one node for
	 * new root.
	 */
	rc = bplus_tree_get_new_node(&new_internal_ino);
	CHECK_RC_ASSERT(rc, EOK);
	new_internal_node = (void *)malloc(NODE_SIZE);
	CHECK_RC_ASSERT((new_internal_node == NULL), 0);

	new_internal_node_path = (char *)malloc(MAX_PATH);
	CHECK_RC_ASSERT((new_internal_node_path == NULL), 0);
	rc = get_path(META_DIR, new_internal_ino, new_internal_node_path);
	CHECK_RC_ASSERT(rc, EOK);

	rc = bplus_tree_get_new_node(&root_internal_ino);
	CHECK_RC_ASSERT(rc, EOK);
	root_internal_node = (void *)malloc(NODE_SIZE);
	CHECK_RC_ASSERT((root_internal_node == NULL), 0);

	root_node_path = (char *)malloc(MAX_PATH);
	CHECK_RC_ASSERT((root_node_path == NULL), 0);
	rc = get_path(META_DIR, root_internal_ino, root_node_path);
	CHECK_RC_ASSERT(rc, EOK);

	rc = read_file_contents(root_node_path,
				root_internal_node,
				READ_FLAGS,
				READ_MODE,
				NODE_SIZE);
	CHECK_RC_ASSERT(rc, EOK);

	rc = read_file_contents(new_internal_node_path,
				new_internal_node,
				READ_FLAGS,
				READ_MODE,
				NODE_SIZE);
	CHECK_RC_ASSERT(rc, EOK);

	block_head = bplus_tree_get_block_head(internal_node);
	nr_keys_internal = NR_KEYS(block_head->nr_items);
	nr_dc_internal = nr_keys_internal + 1;
	nr_keys_to_flow = (nr_keys_internal / 2);
	nr_dc_to_flow = nr_keys_to_flow + 1;
	
	/*
	 * As new_internal_node will be a right neighbor of internal_node, level of
	 * internal_node should be equal to level of internal_node.
	 */
	block_head_new = bplus_tree_get_block_head(new_internal_node);
	block_head_new->level = block_head->level;
	CHECK_RC_ASSERT((block_head_new->level == BTREE_LEAF_LEVEL), 0);
	CHECK_RC_ASSERT(block_head_new->nr_items, 0);

	/*
	 * Lets flow nr_keys to new internal node.
	 */
	for (i = 0; i < nr_keys_to_flow; i++)
	{

		rc = bplus_tree_flow_key(internal_node,
					 new_internal_node,
					 nr_keys_internal,
					 block_head_new->nr_items);
		CHECK_RC_ASSERT(rc, EOK);
		nr_keys_internal--;

	}

	/*
	 * Lets flow nr_dc disk_child to new internal node.
	 */
	src_dc_pos = nr_dc_internal - 1;
	dst_dc_pos = nr_dc_to_flow - 1;
	for (i = 0; i < nr_dc_to_flow; i++)
	{

		rc = bplus_tree_flow_dc(internal_node,
					new_internal_node,
					src_dc_pos,
					dst_dc_pos);
		CHECK_RC_ASSERT(rc, EOK);
		src_dc_pos--;
		dst_dc_pos--;

	}

	/*
	 * Now, it is time to adjust internal nodes to keep B+ tree property intact.
	 * There will be minimum 2 adjustments.
	 * 1. Adjust leaf's parent
	 * 2. Adjust internal node's parent.
	 * #2 could be repeated to go upwards. However, this will be done in next
	 * pass.
	 */

	/*
	 * Figure out right parent node of leaf where the key needs to be inserted.
	 */
	key1 = bplus_tree_get_key(internal_node, 0);
	key2 = bplus_tree_get_key(new_internal_node, 0);

	internal_node_to_insert = new_internal_node;
	tmp_internal_path = new_internal_node_path;
	pos_to_reset = nr_dc_to_flow - 1;
	nr_keys_to_shift = nr_keys_to_flow;
	nr_dc_to_shift = nr_dc_to_flow;

	if ((key.i_ino < key1->i_ino) || (key.i_ino < key2->i_ino))
	{

		internal_node_to_insert = internal_node;
		tmp_internal_path = internal_node_path;

		/*
		 * New key and dc will be inserted into internal_node. So, position
		 * to reset also should be incremented. Otherwise, rebalancing 
		 * would result into data corruption.
		 */
		pos_to_reset = nr_dc_to_flow;
		nr_keys_to_shift = nr_keys_to_flow + 1;
		nr_dc_to_shift = nr_keys_to_shift;

	}
	
	/*
	 * Copy appropriate disk child as per the flow_mode.
	 */
	block_head = bplus_tree_get_block_head(new_leaf_node);
	level = block_head->level;
	bplus_tree_handle_internal_insert(tb,
					  internal_node_to_insert,
					  tmp_internal_path,
					  flow_mode,
					  new_leaf_ino,
					  internal_ino,
					  &key,
					  nr_keys_to_shift,
					  nr_dc_to_shift,
					  level);

	/*
	 * Get key to insert into new root node. It should be new internal
	 * node's first key.
	 * Now, initialize the root node and adjust root node.
	 */
	block_head = bplus_tree_get_block_head(internal_node_to_insert);
	level = block_head->level;

	/*
	 * Reset the last key in internal_node at pos_to_reset position.
	 * Root block is always going to be consistent. So, pass NR_KEYS can be
	 * used safely here.
	 */
	memcpy(&key,
		bplus_tree_get_key(internal_node, pos_to_reset),
		KEY_SIZE);
	bplus_tree_reset_key(internal_node, pos_to_reset);

	block_head = bplus_tree_get_block_head(root_internal_node);
	nr_keys_to_shift = NR_KEYS(block_head->nr_items);
	nr_dc_to_shift = nr_keys_to_shift + 1;

	bplus_tree_handle_internal_insert(tb,
					  root_internal_node,
					  root_node_path,
					  flow_mode,
					  new_internal_ino,
					  root_internal_ino,
					  &key,
					  nr_keys_to_shift,
					  nr_dc_to_shift,
					  level);

	/*
	 * Now, lets add following 3 nodes to respective linked lists : 
	 * 1. new_leaf_node at level 0.
	 * 2. internal_node at level 1.
	 * 3. root_internal_node at level 2.
	 */
	bplus_tree_attach_neighbor(tb,
				   new_leaf_node,
				   new_leaf_node_path,
				   flow_mode,
				   BTREE_LEAF_LEVEL, 0);

	bplus_tree_attach_neighbor(tb,
				   new_internal_node,
				   new_internal_node_path,
				   flow_mode,
				   (BTREE_LEAF_LEVEL + 1), 0);

	bplus_tree_attach_neighbor(tb,
				   root_internal_node,
				   root_node_path,
				   flow_mode,
				   (BTREE_LEAF_LEVEL + 2), 0);

	free(root_node_path);
	free(root_internal_node);
	free(new_internal_node_path);
	free(new_internal_node);

	return rc;

}

/*
 * During re-balancing of B+ tree, root could be changed. This function writes file
 * inode of root into root_path.
 */
int bplus_tree_adjust_root(char *root_path, ino_t new_root_ino)
{

	int rc = EOK;

	CHECK_RC_ASSERT((root_path == NULL), 0);

	rc =  write_file_contents(root_path, WRITE_FLAGS, WRITE_MODE,
			&new_root_ino, sizeof(ino_t));
	return rc;

}

/*
 * This function handles all the nitty-gritties of flowing item.
 */
int bplus_tree_flow_item_handle(bplus_tree_balance_st *tb,
				item_st *item,
				ino_t new_leaf_ino,
				bool flow_mode)
{

	int i, rc = EOK;
	char *internal_node_path, *new_leaf_node_path, *tmp_path;
	void *leaf_node, *new_leaf_node, *node, *internal_node;
	void *parent_node;
	bplus_tree_traverse_path_st *traverse_path;
	block_head_st *block_head;
	ino_t internal_ino;
	bool free_internal_node = FALSE;

	traverse_path = tb->tb_path;

	new_leaf_node_path = (char *)malloc(MAX_PATH);
	CHECK_RC_ASSERT((new_leaf_node_path == NULL), 0);

	new_leaf_node = (void *)malloc(NODE_SIZE);
	CHECK_RC_ASSERT((new_leaf_node == NULL), 0);

	rc = get_path(META_DIR, new_leaf_ino, new_leaf_node_path);
	CHECK_RC_ASSERT(rc, EOK);

	rc = read_file_contents(new_leaf_node_path,
				new_leaf_node,
				READ_FLAGS,
				READ_MODE,
				NODE_SIZE);
	CHECK_RC_ASSERT(rc, EOK);

	/*
	 * If a node at level up to BTREE_LEAF_LEVEL is not present, then allocate
	 * it. 
	 * Else, figure out internal node path from traverse_path.
	 */
	parent_node = bplus_tree_get_node_path(traverse_path, BTREE_LEAF_LEVEL + 1);
	if (parent_node == NULL)
	{

		rc = bplus_tree_get_new_node(&internal_ino);
		CHECK_RC_ASSERT(rc, EOK);

		internal_node_path = (char *)malloc(MAX_PATH);
		CHECK_RC_ASSERT((internal_node_path == NULL), 0);

		rc = get_path(META_DIR, internal_ino, internal_node_path);
		CHECK_RC_ASSERT(rc, EOK);

		internal_node = (void *)malloc(NODE_SIZE);
		CHECK_RC_ASSERT((internal_node == NULL), 0);

		rc = read_file_contents(internal_node_path,
					internal_node,
					READ_FLAGS,
					READ_MODE,
					NODE_SIZE);
		CHECK_RC_ASSERT(rc, EOK);
		free_internal_node = TRUE;

	}
	else
	{

		internal_node_path = bplus_tree_get_pe_path_path(
					traverse_path,
					(BTREE_LEAF_LEVEL + 1));
		rc = is_path_present(internal_node_path, &internal_ino);
		CHECK_RC_ASSERT(rc, EOK);

		internal_node = bplus_tree_get_node_path(traverse_path,
					(BTREE_LEAF_LEVEL + 1));

	}

	leaf_node = bplus_tree_get_node_path(traverse_path, BTREE_LEAF_LEVEL);

	/*
	 * Case 1 targets following :
	 * If parent node of the leaf is not full then, following condition
	 * will be executed.
	 */
	block_head = bplus_tree_get_block_head(internal_node);
	if (block_head->nr_items < (MAX_KEYS + MAX_DC))
	{

		rc = bplus_tree_flow_item_case1(tb, flow_mode, leaf_node,
			new_leaf_node, internal_node, item, 
			internal_node_path,
			new_leaf_node_path,
			new_leaf_ino, internal_ino);

	}
	/*
	 * Case 2 targets following :
	 * If parent node of the leaf is full then, following condition
	 * will be executed.
	 */
	else
	{

		rc = bplus_tree_flow_item_case2(tb, flow_mode, leaf_node,
			new_leaf_node, internal_node, item, 
			internal_node_path,
			new_leaf_node_path,
			new_leaf_ino, internal_ino);

	}

	if (free_internal_node == TRUE)
	{

		free(internal_node);
		free(internal_node_path);
	}

	free(new_leaf_node);
	free(new_leaf_node_path);
	return rc;

}

/*
 * This function is used for flowing a key from one node to another.
 * This is only used for internal node.
 */
int bplus_tree_flow_key(void *src, void *dest, int nr_keys_src, int nr_keys_dest)
{

	int rc = EOK;
	block_head_st *block_head_src, *block_head_dest;
	b_plus_tree_key_t *src_key, *dest_key;
	int i;

	CHECK_RC_ASSERT((src == NULL), 0);
	CHECK_RC_ASSERT((dest == NULL), 0);

	block_head_src = bplus_tree_get_block_head(src);
	block_head_dest = bplus_tree_get_block_head(dest);

	/*
	 * Take last key in src buffer.
	 * Shift dest keys right one by one.
	 * Take dest_key at empty location.
	 */
	src_key = bplus_tree_get_key(src, (nr_keys_src - 1));
	i = bplus_tree_shift_right_keys(dest, src_key, nr_keys_dest);
	dest_key = bplus_tree_get_key(dest, i);
	CHECK_RC_ASSERT(dest_key->i_ino, 0);
	memcpy(dest_key, src_key, KEY_SIZE);

	/*
	 * Adjust counters.
	 */
	block_head_dest->nr_items += 1;
	block_head_src->nr_items -= 1;
	block_head_dest->free_space -= KEY_SIZE;
	block_head_src->free_space += KEY_SIZE;

	return rc;

}

/*
 * This function is used for flowing disk child from one node to another.
 * This is used for internal node.
 */
int bplus_tree_flow_dc(
			void *src,
			void *dest,
			uint16_t src_pos_dc,
			uint16_t dst_pos_dc)
{

	int rc = EOK;
	block_head_st *block_head_src, *block_head_dest;
	disk_child_st *src_dc, *dest_dc;

	CHECK_RC_ASSERT((src == NULL), 0);
	CHECK_RC_ASSERT((dest == NULL), 0);

	block_head_src = bplus_tree_get_block_head(src);
	block_head_dest = bplus_tree_get_block_head(dest);

	/*
	 * Take src_dc at position in src buffer.
	 * Take dest_dc at position in dest buffer.
	 * Do a flow
	 * Reset the src_dc as 0. 
	 */
	src_dc = bplus_tree_get_dc(src, src_pos_dc);
	dest_dc = bplus_tree_get_dc(dest, dst_pos_dc);
	CHECK_RC_ASSERT(dest_dc->i_ino, 0);
	memcpy(dest_dc, src_dc, DC_SIZE);
	memset(src_dc, 0, DC_SIZE);

	/*
	 * Adjust counters.
	 */
	block_head_dest->nr_items += 1;
	block_head_src->nr_items -= 1;
	block_head_dest->free_space -= DC_SIZE;
	block_head_src->free_space += DC_SIZE;

	return rc;

}

/*
 * This function is used for flowing an item from one node to another.
 * This is only used for leaf node.
 */
int bplus_tree_flow_item(void *src, void *dest)
{

	int rc = EOK;
	block_head_st *block_head_src, *block_head_dest;
	item_st *src_item, *dest_item;
	int i;

	CHECK_RC_ASSERT((src == NULL), 0);
	CHECK_RC_ASSERT((dest == NULL), 0);

	block_head_src = bplus_tree_get_block_head(src);
	block_head_dest = bplus_tree_get_block_head(dest);

	/*
	 * Take last item in src buffer.
	 * Shift dest items right one by one.
	 * Take dest_item at empty location.
	 */
	src_item = bplus_tree_get_item(src, (block_head_src->nr_items - 1));
	i = bplus_tree_shift_right(dest, src_item, block_head_dest->nr_items);
	dest_item = bplus_tree_get_item(dest, i);
	memcpy(dest_item, src_item, ITEM_SIZE);
	memset(src_item, 0, ITEM_SIZE);

	/*
	 * Adjust counters.
	 */
	block_head_dest->nr_items += 1;
	block_head_src->nr_items -= 1;
	block_head_dest->free_space -= ITEM_SIZE;
	block_head_src->free_space += ITEM_SIZE;

	return rc;

}

/*
 * This function allocates new b+ tree node from allocator and formats it.
 * It also returns inode no. of newly created node.
 */
int bplus_tree_get_new_node(ino_t *i_ino)
{

	char *path;
	int rc = EOK;
	ino_t node_num;

	path = (char *)malloc(MAX_PATH);
	CHECK_RC_ASSERT((path == NULL), 0);

	rc = bplus_tree_allocate_node_num(&node_num);
	if (rc != EOK)
	{

		free(path);
		return rc;

	}

	snprintf(path, MAX_PATH, "%s/%d", META_DIR, (int)node_num);
	rc = bplus_tree_allocate_node(path, i_ino);
	if (rc != EOK)
	{

		free(path);
		return rc;

	}

	free(path);
	return rc;

}

/*
 * This is allocator of internal and leaf nodes of b+ tree.
 */
int bplus_tree_allocate_node_num(ino_t *node_num)
{

	int rc = EOK;
	ino_t allocator_num;

	CHECK_RC_ASSERT((node_num == NULL), 0);
	*node_num = 0;

	/*
	 * If allocator file is not present, create it with number as 0 in it.
	 */
	rc = is_path_present(ALLOCATOR, &allocator_num);
	if (rc == ENOENT)
	{

		rc = bplus_tree_initialize_allocator();
		if (rc != EOK)
		{
			return rc;
		}

	}
	CHECK_RC_ASSERT(rc, EOK);

	rc = read_file_contents(ALLOCATOR,
			   &allocator_num,
			   READ_FLAGS,
			   READ_MODE,
			   sizeof(ino_t));
	if (rc != EOK)
	{
		return rc;
	}

	allocator_num += 1;
	rc = write_file_contents(ALLOCATOR,
			   WRITE_FLAGS,
			   WRITE_MODE,
			   &allocator_num,
			   sizeof(ino_t));
	if (rc != EOK)
	{
		return rc;
	}

	*node_num = allocator_num;
	return rc;

}

/*
 * This function initializes allocator file to zero.
 */
int bplus_tree_initialize_allocator()
{

	int fd = -1;
	int rc = EOK;
	ino_t i_ino = 0;

	fd = open(ALLOCATOR, OPEN_FLAGS, OPEN_MODE);
	if (fd == -1)
	{

		rc = errno;
		return rc;

	}
	close(fd);

	rc = write_file_contents(ALLOCATOR,
				 WRITE_FLAGS,
				 WRITE_MODE,
				 &i_ino,
				 sizeof(ino_t));
	return rc;

}

/*
 * This function gets left delimiting key of the item/key.
 */
b_plus_tree_key_t *bplus_tree_get_lkey(
			bplus_tree_traverse_path_st *traverse_path,
			uint8_t level)
{

	
	b_plus_tree_key_t *lkey = NULL;
	void *buf, *parent_node;
	uint16_t path_length;
	int pe_position;

	CHECK_RC_ASSERT((traverse_path == NULL), 0);

	buf = bplus_tree_get_node_path(traverse_path, level);
	CHECK_RC_ASSERT((buf == NULL), 0);

	path_length = traverse_path->path_length;
	while (level < path_length)
	{

		/*
		 * Go to parent, and take key located at (pe_position -1).
		 */
		parent_node = bplus_tree_get_parent_node_path(traverse_path, level);
		pe_position = bplus_tree_get_pos_path(traverse_path, (level + 1));
		level++;

		if (pe_position == 0)
		{
			continue;
		}

		lkey = bplus_tree_get_key(parent_node, (pe_position-1));

	}

	return lkey;

}

/*
 * This function gets right delimiting key of the item/key.
 */
b_plus_tree_key_t *bplus_tree_get_rkey(
			bplus_tree_traverse_path_st *traverse_path,
			uint8_t level)
{

	b_plus_tree_key_t *rkey = NULL;
	void *buf, *parent_node;
	uint16_t path_length;
	int pe_position;
	block_head_st *block_head;

	CHECK_RC_ASSERT((traverse_path == NULL), 0);

	buf = bplus_tree_get_node_path(traverse_path, level);
	CHECK_RC_ASSERT((buf == NULL), 0);

	path_length = traverse_path->path_length;
	while (level < path_length)
	{

		/*
		 * Go to parent, and take key located at (pe_position).
		 */
		parent_node = bplus_tree_get_parent_node_path(traverse_path, level);
		pe_position = bplus_tree_get_pos_path(traverse_path, (level + 1));
		block_head = bplus_tree_get_block_head(parent_node);

		/*
		 * If it is non-leaf node, nr_items counts keys and dc.
		 * As no. of keys needs to be considered, NR_KEYS will be
		 * calculated.
		 */
		if (block_head->level == BTREE_LEAF_LEVEL)
		{

			level++;
			if (pe_position == block_head->nr_items)
			{
				continue;
			}

		}
		else
		{

			level++;
			if (pe_position == NR_KEYS(block_head->nr_items))
			{
				continue;
			}

		}

		rkey = bplus_tree_get_key(parent_node, (pe_position + 1));

	}

	return rkey;

}

/*
 * This function initializes the tree_balance structure.
 */
bplus_tree_balance_st *bplus_tree_init_tb(
			char *root_path,
			bplus_tree_traverse_path_st *traverse_path)
{

	int i;
	int rc = EOK;
	void *neighbor = NULL;
	bplus_tree_balance_st *tb = NULL;
	path_element_st *path_element;
	char *path;

	CHECK_RC_ASSERT((traverse_path == NULL), 0);

	tb = (bplus_tree_balance_st *)malloc(TB_SIZE);
	CHECK_RC_ASSERT((tb == NULL), 0);
	memset(tb, 0, TB_SIZE);

	tb->tb_root_path = root_path;

	neighbor = (void *)malloc(NODE_SIZE);
	CHECK_RC_ASSERT((neighbor == NULL), 0);

	path_element = (path_element_st *)malloc(PE_SIZE);
	CHECK_RC_ASSERT((path_element == NULL), 0);

	path = (char *)malloc(MAX_PATH);
	CHECK_RC_ASSERT((path == NULL), 0);

	tb->tb_path = traverse_path;

	for (i = 0; i <= traverse_path->path_length; i++)
	{

		/*
		 * Get only one left/right neighbor at a time at a level.
		 */
		tb->tb_left[i] = NULL;
		rc = bplus_tree_get_neighbor(traverse_path, i, neighbor, 
					    LEFT, path);
		if (rc = EOK)
		{

			bplus_tree_fill_pe(neighbor, path, 0, path_element);
			tb->tb_left[i] = sll_insert_node_pos(
					 tb->tb_left[i],
					 path_element,
					 tb->tb_nr_left[i],
					 PE_SIZE);
			(tb->tb_nr_left[i])++;

		}

		tb->tb_right[i] = NULL;
		rc = bplus_tree_get_neighbor(traverse_path, i, neighbor,
					    RIGHT, path);
		if (rc = EOK)
		{

			bplus_tree_fill_pe(neighbor, path, 0, path_element);
			tb->tb_right[i] = sll_insert_node_pos(
					 tb->tb_right[i],
					 path_element,
					 tb->tb_nr_right[i],
					 PE_SIZE);
			(tb->tb_nr_right[i])++;

		}

	}

	free(path);
	free(neighbor);
	free(path_element);
	return tb;

}

/*
 * This function deinitializes the tree balance structure.
 */
int bplus_tree_deinit_tb(bplus_tree_balance_st *tb)
{

	int i, rc = EOK;

	for (i = 0; i < BPLUS_MAX_HEIGHT; i++)
	{

		sll_remove_sll(tb->tb_left[i]);
		sll_remove_sll(tb->tb_right[i]);
		tb->tb_left[i] = NULL;
		tb->tb_right[i] = NULL;

	}

	tb->tb_path = NULL;
	free(tb);
	return rc;

}

/*
 * This function returns in-memory contents of left/right neighbor at level from 
 * traverse_path.
 */
int bplus_tree_get_neighbor(
		bplus_tree_traverse_path_st *traverse_path,
		uint8_t level, void *neighbor, neighbor_et type, char *path)
{

	int rc = EOK;
	b_plus_tree_key_t *key = NULL;
	void *parent_node = NULL;
	disk_child_st *dc = NULL;
	int pe_position;

	CHECK_RC_ASSERT((traverse_path == NULL), 0);
	CHECK_RC_ASSERT((neighbor == NULL), 0);

	/*
	 * Figure out the left/right delimiting key at the level. If lkey is 
	 * non-NULL, then find out the parent of the node at level.
	 * Find out disk_child at (position - 1)
	 * Read contents of file pointed by disk child.
	 */
	if (type == LEFT)
	{
		key = bplus_tree_get_lkey(traverse_path, level);
	}
	else
	{
		key = bplus_tree_get_rkey(traverse_path, level);
	}

	if (key == NULL)
	{
		return ENOENT;
	}

	parent_node = bplus_tree_get_parent_node_path(traverse_path, level);
	CHECK_RC_ASSERT((parent_node == NULL), 0);
	pe_position = bplus_tree_get_pos_path(traverse_path, (level + 1));

	if (type == LEFT)
	{
		dc = bplus_tree_get_dc(parent_node, (pe_position - 1));
	}
	else
	{
		dc = bplus_tree_get_dc(parent_node, (pe_position + 1));
	}
	
	CHECK_RC_ASSERT((dc == NULL), 0);
	CHECK_RC_ASSERT((dc->i_ino == 0), 0);

	rc = get_path(META_DIR, dc->i_ino, path);
	if (rc == EOK)
	{

		memset(neighbor, 0, NODE_SIZE);
		rc = read_file_contents(path, neighbor, READ_FLAGS,
				   READ_MODE, NODE_SIZE);

	}

	return rc;

}

/*
 * This function determines flow direction to which item should flow. If position is
 * beyond midway, then flow item to the right.
 */
bool bplus_tree_find_flow_dir(int pe_position)
{

	bool flow = LEFT;

	if ((MAX_ITEMS / 2) <= pe_position)
	{
		flow = RIGHT;
	}

	return flow;	

}

/*
 * This function zeroes out a key at a position. 
 */
int bplus_tree_reset_key(void *internal_node, int position)
{

	int rc = EOK;
	block_head_st *block_head;
	b_plus_tree_key_t *key;

	CHECK_RC_ASSERT((internal_node == NULL), 0);

	block_head = bplus_tree_get_block_head(internal_node);
	CHECK_RC_ASSERT((block_head->level == BTREE_LEAF_LEVEL), 0);

	key = bplus_tree_get_key(internal_node, position);
	memset(key, 0, KEY_SIZE);

	(block_head->nr_items)--;
	block_head->free_space += KEY_SIZE;

	return rc;

}

