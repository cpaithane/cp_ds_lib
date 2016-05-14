#include "b_plus_tree_interface.h"

#define DATA ".data"

/*
 * This function forms a file name using iterator. Strictly, used for unit test 
 * purposes.
 */
void get_file_name(char *file_name, int i)
{

	memset(file_name, 0, MAX_PATH);
	snprintf(file_name, MAX_PATH, "%s/%d", DATA, i);
	file_name[strlen(file_name)] = '\0';

}

/*
 * This test case tests simple insertion/lookup of 2048 objects inside b+ tree.
 */
int bplus_test_case1()
{

	int rc = EOK;
	char *file_name = NULL;
	int i, fd, position;
	ino_t i_ino;
	bplus_tree_traverse_path_st *traverse_path = NULL;
	b_plus_tree_key_t *key = NULL;
	void *leaf_node;
	item_st *item;
	block_head_st *block_head = NULL;

	rc = bplus_tree_init(META_DIR, ROOT, TRUE);
	CHECK_RC_ASSERT(rc, EOK);

	file_name = (char *)malloc(MAX_PATH);
	CHECK_RC_ASSERT((file_name == NULL), 0);

	traverse_path = (bplus_tree_traverse_path_st *)malloc(TRAVERSE_PATH_SIZE);
	CHECK_RC_ASSERT((traverse_path == NULL), 0);
	memset(traverse_path, 0, sizeof(bplus_tree_traverse_path_st));

	key = (b_plus_tree_key_t *)malloc(KEY_SIZE);
	CHECK_RC_ASSERT((key == NULL), 0);
	memset(key, 0, KEY_SIZE);

	if (is_path_present(DATA, &i_ino) == ENOENT)
	{

		mkdir (DATA, 0744);
		for (i = 0; i < MAX_ITEMS; i++)
		{

			get_file_name(file_name, i);
			fd = open(file_name, OPEN_FLAGS, OPEN_MODE);
			close(fd);

		}

	}

	/*
	 * Insert the inode no. of file into b+ tree.
	 */
	for (i = 0; i < MAX_ITEMS; i++)
	{

		get_file_name(file_name, i);
		rc = bplus_tree_insert(ROOT, file_name);
		CHECK_RC_ASSERT(rc, EOK);

	}

	/*
	 * Now, search the items in b+ tree.
	 */
	printf("Data in B+ tree\n");
	for (i = 0; i < MAX_ITEMS; i++)
	{
	
		get_file_name(file_name, i);
		rc = is_path_present(file_name, &i_ino);
		CHECK_RC_ASSERT(rc, EOK);

		rc = bplus_form_key(file_name, key);
		CHECK_RC_ASSERT(rc, EOK);

		rc = bplus_tree_search_key(ROOT, key, traverse_path);
		CHECK_RC_ASSERT(rc, EEXIST);

		leaf_node = bplus_tree_get_leaf_path(traverse_path);
		CHECK_RC_ASSERT((leaf_node == NULL), 0);

		position = bplus_tree_get_pos_path(traverse_path, BTREE_LEAF_LEVEL);
		item = bplus_tree_get_item(leaf_node, position);
		CHECK_RC_ASSERT(item->i_ino, i_ino);
	
		printf("Filename = %s, key = %u\n",
		    file_name,
		    (uint32_t)item->i_ino);

	}

	/*
	 * Delete the inode no. of file into b+ tree.
	 * Then, lookup inside b+ tree for the key just deleted.
	 */
	for (i = 0; i < MAX_ITEMS; i++)
	{

		get_file_name(file_name, i);
		rc = bplus_tree_delete(ROOT, file_name);
		CHECK_RC_ASSERT(rc, EOK);

		rc = bplus_form_key(file_name, key);
		CHECK_RC_ASSERT(rc, EOK);

		rc = bplus_tree_search_key(ROOT, key, traverse_path);
		CHECK_RC_ASSERT(rc, ENOENT);

		printf("Deleted filename = %s, key = %u\n",
		    file_name,
		    (uint32_t)key->i_ino);
		
	}

	/*
	 * Lets get the b+ tree leaf block and compare it's contents with 0.
	 */
	rc = bplus_tree_search_key(ROOT, key, traverse_path);
	CHECK_RC_ASSERT(rc, ENOENT);

	for (i = 0; i < MAX_ITEMS; i++)
	{

		leaf_node = bplus_tree_get_leaf_path(traverse_path);
		item = bplus_tree_get_item(leaf_node, i);
		CHECK_RC_ASSERT(item->i_ino, 0);

	}

	block_head = bplus_tree_get_block_head(leaf_node);
	CHECK_RC_ASSERT(block_head->nr_items, 0);
	CHECK_RC_ASSERT(block_head->free_space, (NODE_SIZE - BLOCK_HEAD_SIZE));

	rc = bplus_tree_deinit(META_DIR);
	CHECK_RC_ASSERT(rc, EOK);

	free(key);
	bplus_tree_free_traverse_path(traverse_path);
	free(file_name);
	return rc;

}

/*
 * This function is workhorse for execution of test cases.
 */
int bplus_tc_execute()
{

	int rc = EOK;

	rc = bplus_test_case1();

	return rc;

}

