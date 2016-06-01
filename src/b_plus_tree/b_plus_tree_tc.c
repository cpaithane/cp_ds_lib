#include "b_plus_tree_interface.h"

#define DATA ".data"
#define MAX_INPUT_ITEMS 16515

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
 * This function created MAX_INPUT_ITEMS no. of files under .data directory.
 */
int bplus_create_data()
{

	ino_t i_ino;
	int fd, i, rc = EOK;
	char *file_name = NULL;

	file_name = (char *)malloc(MAX_PATH);
	CHECK_RC_ASSERT((file_name == NULL), 0);

	if (is_path_present(DATA, &i_ino) == ENOENT)
	{

		mkdir (DATA, 0744);
		for (i = 0; i < MAX_INPUT_ITEMS; i++)
		{

			get_file_name(file_name, i);
			fd = open(file_name, OPEN_FLAGS, OPEN_MODE);
			if (fd == -1)
			{

				rc = errno;
				break;

			}
			close(fd);

		}

	}

	free(file_name);
	return rc;

}

/*
 * This function inserts keys in b+ tree in either ascending or descending manner.
 */
int bplus_tc_insert_keys(bool is_ascending)
{

	int i, rc = EOK;
	bplus_tree_traverse_path_st *traverse_path = NULL;
	char *file_name = NULL;
	b_plus_tree_key_t *key = NULL;

	file_name = (char *)malloc(MAX_PATH);
	CHECK_RC_ASSERT((file_name == NULL), 0);
	
	key = (b_plus_tree_key_t *)malloc(KEY_SIZE);
	CHECK_RC_ASSERT((key == NULL), 0);
	memset(key, 0, KEY_SIZE);

	if (is_ascending == TRUE)
	{


		for (i = 0; i < MAX_INPUT_ITEMS; i++)
		{

			get_file_name(file_name, i);
			rc = bplus_tree_insert(ROOT, file_name);
			CHECK_RC_ASSERT(rc, EOK);
			rc = bplus_form_key(file_name, key);
			CHECK_RC_ASSERT(rc, EOK);

			traverse_path =
			(bplus_tree_traverse_path_st *)malloc(TRAVERSE_PATH_SIZE);
			CHECK_RC_ASSERT((traverse_path == NULL), 0);
			memset(traverse_path, 0, TRAVERSE_PATH_SIZE);

			rc = bplus_tree_search_key(ROOT, key, traverse_path);
			CHECK_RC_ASSERT(rc, EEXIST);
			bplus_tree_free_traverse_path(traverse_path);

		}
	}
	else
	{

		for (i = MAX_INPUT_ITEMS; i >= 0; i--)
		{

			get_file_name(file_name, i);
			rc = bplus_tree_insert(ROOT, file_name);
			CHECK_RC_ASSERT(rc, EOK);
			rc = bplus_form_key(file_name, key);
			CHECK_RC_ASSERT(rc, EOK);

			traverse_path =
			(bplus_tree_traverse_path_st *)malloc(TRAVERSE_PATH_SIZE);
			CHECK_RC_ASSERT((traverse_path == NULL), 0);
			memset(traverse_path, 0, TRAVERSE_PATH_SIZE);
			rc = bplus_tree_search_key(ROOT, key, traverse_path);
			CHECK_RC_ASSERT(rc, EEXIST);
			bplus_tree_free_traverse_path(traverse_path);

		}

	}

	free(key);
	free(file_name);
	return rc;

}

/*
 * This function searches keys in bplus tree.
 */
int bplus_tc_search_keys()
{


	int i, position, rc = EOK;
	char *file_name;
	bplus_tree_traverse_path_st *traverse_path = NULL;
	b_plus_tree_key_t *key = NULL;
	ino_t i_ino;
	void *leaf_node;
	item_st *item;

	file_name = (char *)malloc(MAX_PATH);
	CHECK_RC_ASSERT((file_name == NULL), 0);

	key = (b_plus_tree_key_t *)malloc(KEY_SIZE);
	CHECK_RC_ASSERT((key == NULL), 0);
	memset(key, 0, KEY_SIZE);

	printf("Data in B+ tree\n");
	for (i = 0; i < MAX_INPUT_ITEMS; i++)
	{
	
		get_file_name(file_name, i);
		rc = is_path_present(file_name, &i_ino);
		CHECK_RC_ASSERT(rc, EOK);

		rc = bplus_form_key(file_name, key);
		CHECK_RC_ASSERT(rc, EOK);

		traverse_path =
		(bplus_tree_traverse_path_st *)malloc(TRAVERSE_PATH_SIZE);
		CHECK_RC_ASSERT((traverse_path == NULL), 0);
		memset(traverse_path, 0, TRAVERSE_PATH_SIZE);
		rc = bplus_tree_search_key(ROOT, key, traverse_path);
		CHECK_RC_ASSERT(rc, EEXIST);

		leaf_node = bplus_tree_get_node_path(traverse_path,
						    BTREE_LEAF_LEVEL);
		CHECK_RC_ASSERT((leaf_node == NULL), 0);

		position = bplus_tree_get_pos_path(traverse_path, BTREE_LEAF_LEVEL);
		item = bplus_tree_get_item(leaf_node, position);
		CHECK_RC_ASSERT(item->i_ino, i_ino);
	
		printf("Filename = %s, key = %u\n",
		    file_name,
		    (uint32_t)item->i_ino);

		bplus_tree_free_traverse_path(traverse_path);

	}

	free(file_name);
	free(key);
	return rc;

}

/*
 * This function deletes keys from bplus tree and searches the same key.
 */
int bplus_tc_delete_keys(int position)
{

	int i, rc = EOK;
	char *file_name;
	bplus_tree_traverse_path_st *traverse_path = NULL;
	b_plus_tree_key_t *key = NULL;
	b_plus_tree_key_t first_key;
	void *leaf_node;
	item_st *item, *first_item;
	block_head_st *block_head = NULL;

	file_name = (char *)malloc(MAX_PATH);
	CHECK_RC_ASSERT((file_name == NULL), 0);

	key = (b_plus_tree_key_t *)malloc(KEY_SIZE);
	CHECK_RC_ASSERT((key == NULL), 0);
	memset(key, 0, KEY_SIZE);

	/*
	 * Lets figure out the first key in leaf node.
	 */
	get_file_name(file_name, position);
	rc = bplus_form_key(file_name, key);
	CHECK_RC_ASSERT(rc, EOK);

	traverse_path =
		(bplus_tree_traverse_path_st *)malloc(TRAVERSE_PATH_SIZE);
	CHECK_RC_ASSERT((traverse_path == NULL), 0);
	memset(traverse_path, 0, TRAVERSE_PATH_SIZE);

	rc = bplus_tree_search_key(ROOT, key, traverse_path);
	CHECK_RC_ASSERT(rc, EEXIST);

	leaf_node = bplus_tree_get_node_path(traverse_path,
					    BTREE_LEAF_LEVEL);
	CHECK_RC_ASSERT((leaf_node == NULL), 0);

	first_item = bplus_tree_get_item(leaf_node, 0);
	first_key.i_ino = first_item->i_ino;

	rc = get_path(DATA, first_key.i_ino, file_name);
	CHECK_RC_ASSERT(rc, EOK);
	bplus_tree_free_traverse_path(traverse_path);

	rc = bplus_tree_delete(ROOT, file_name);
	CHECK_RC_ASSERT(rc, EOK);

	traverse_path =
		(bplus_tree_traverse_path_st *)malloc(TRAVERSE_PATH_SIZE);
	CHECK_RC_ASSERT((traverse_path == NULL), 0);
	memset(traverse_path, 0, TRAVERSE_PATH_SIZE);

	rc = bplus_tree_search_key(ROOT, &first_key, traverse_path);
	CHECK_RC_ASSERT(rc, ENOENT);
	bplus_tree_free_traverse_path(traverse_path);

#ifdef BPLUS_DELETE_ADVANCE

	/*
	 * Delete the inode no. of file into b+ tree.
	 * Then, lookup inside b+ tree for the key just deleted.
	 * Currently, testing only for single key deletion.
	 */
	for (i = 1; i < 2; i++)
	{

		get_file_name(file_name, i);
		rc = bplus_form_key(file_name, key);
		CHECK_RC_ASSERT(rc, EOK);

		if (key->i_ino == first_key.i_ino)
		{
			continue;
		}

		rc = bplus_tree_delete(ROOT, file_name);
		CHECK_RC_ASSERT(rc, EOK);

		traverse_path =
			(bplus_tree_traverse_path_st *)malloc(TRAVERSE_PATH_SIZE);
		CHECK_RC_ASSERT((traverse_path == NULL), 0);
		memset(traverse_path, 0, TRAVERSE_PATH_SIZE);

		rc = bplus_tree_search_key(ROOT, key, traverse_path);
		CHECK_RC_ASSERT(rc, ENOENT);
		bplus_tree_free_traverse_path(traverse_path);

		printf("Deleted filename = %s, key = %u\n",
		    file_name,
		    (uint32_t)key->i_ino);

	}

#endif

	rc = EOK;
	free(file_name);
	free(key);
	
	return rc;

}

/*
 * This test case tests simple insertion/lookup of 16515 objects inside b+ tree.
 */
int bplus_test_case1(bool is_ascending)
{

	int rc = EOK;

	rc = bplus_create_data();
	CHECK_RC_ASSERT(rc, EOK);

	rc = bplus_tree_init(META_DIR, ROOT, TRUE);
	CHECK_RC_ASSERT(rc, EOK);

	/*
	 * Insert the inode no. of file into b+ tree.
	 */
	bplus_tc_insert_keys(is_ascending);

	/*
	 * Now, search the items in b+ tree.
	 */
	bplus_tc_search_keys();

	/*
	 * Now, delete keys from B+ tree.
	 */
	rc = bplus_tc_delete_keys(0);
	CHECK_RC_ASSERT(rc, EOK);

	rc = bplus_tc_delete_keys(130);
	CHECK_RC_ASSERT(rc, EOK);

	rc = bplus_tree_deinit(META_DIR);
	CHECK_RC_ASSERT(rc, EOK);

	return rc;

}

/*
 * This function is workhorse for execution of test cases.
 */
int bplus_tc_execute()
{

	int rc = EOK;
	bool is_ascending;

	is_ascending = TRUE;
	rc = bplus_test_case1(is_ascending);

	is_ascending = FALSE;
	rc = bplus_test_case1(is_ascending);
	return rc;

}

