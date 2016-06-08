/*
 * This file exports four B+ tree APIs : 
 * 1. Insert
 * 2. Delete
 * 3. Search
 */

#include "b_plus_tree_interface.h"

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

