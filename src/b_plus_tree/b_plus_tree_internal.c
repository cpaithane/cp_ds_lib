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
 * This function returns byte position of first key in the internal block.
 */
int bplus_tree_first_dc_pos(void *buf)
{

	int position;
	block_head_st *block_head = NULL;

	CHECK_RC_ASSERT((buf == NULL), 0);

	block_head = bplus_tree_get_block_head(buf);
	position = BLOCK_HEAD_SIZE + (block_head->nr_items * KEY_SIZE);
	return position;

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

	void *node = NULL;
	char *pe_path = NULL;
	int len;

	CHECK_RC_ASSERT((path == NULL), 0);

	node = (void *)malloc(NODE_SIZE);
	CHECK_RC_ASSERT((node == NULL), 0);

	len = strlen(path);
	pe_path = (char *)malloc(len);
	CHECK_RC_ASSERT((pe_path == NULL), 0);

	memset(node, 0, NODE_SIZE);
	memcpy(node, buf, NODE_SIZE);

	memset(pe_path, 0, len);
	memcpy(pe_path, path, len);
	pe_path[len] = '\0';
	CHECK_RC_ASSERT(len, strlen(pe_path));

	traverse_path->path_elements[level].pe_node = node;
	traverse_path->path_elements[level].pe_position = position;
	traverse_path->path_elements[level].pe_path = pe_path;

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
	(block_head->nr_items * KEY_SIZE) + (position * DC_SIZE));
	
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

	b_plus_tree_key_t *key = NULL;
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
 * This function gets leaf node from traverse_path.
 */
void *bplus_tree_get_leaf_path(bplus_tree_traverse_path_st *traverse_path)
{

	CHECK_RC_ASSERT((traverse_path == NULL), 0);
	return (traverse_path->path_elements[BTREE_LEAF_LEVEL].pe_node);

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

