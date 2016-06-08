#include "b_plus_tree_interface.h"

#define DATA ".data\0"
#define MAX_INPUT_ITEMS 16520

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
 * This function handles insertion into B+ tree.
 */
int bplus_tree_tc_insert_handle(int i)
{

	int rc = EOK;
	bplus_tree_traverse_path_st *traverse_path = NULL;
	char *file_name = NULL;
	b_plus_tree_key_t *key = NULL;

	file_name = (char *)malloc(MAX_PATH);
	CHECK_RC_ASSERT((file_name == NULL), 0);
	
	key = (b_plus_tree_key_t *)malloc(KEY_SIZE);
	CHECK_RC_ASSERT((key == NULL), 0);
	memset(key, 0, KEY_SIZE);

	get_file_name(file_name, i);
	rc = bplus_form_key(file_name, key);
	CHECK_RC_ASSERT(rc, EOK);
	rc = bplus_tree_insert(ROOT, key);
	CHECK_RC_ASSERT(rc, EOK);

	traverse_path =
		(bplus_tree_traverse_path_st *)malloc(TRAVERSE_PATH_SIZE);
	CHECK_RC_ASSERT((traverse_path == NULL), 0);
	memset(traverse_path, 0, TRAVERSE_PATH_SIZE);

	rc = bplus_tree_search_key(ROOT, key, traverse_path);
	CHECK_RC_ASSERT(rc, EEXIST);
	bplus_tree_free_traverse_path(traverse_path);

	rc = EOK;
	free(key);
	free(file_name);
	return rc;

}

/*
 * This function inserts keys in b+ tree in either ascending or descending manner.
 */
int bplus_tc_insert_keys(bool is_ascending)
{

	int i, rc = EOK;

	if (is_ascending == TRUE)
	{

		for (i = 0; i < MAX_INPUT_ITEMS; i++)
		{

			rc = bplus_tree_tc_insert_handle(i);
			CHECK_RC_ASSERT(rc, EOK);

		}
	}
	else
	{

		for (i = MAX_INPUT_ITEMS; i >= 0; i--)
		{

			rc = bplus_tree_tc_insert_handle(i);
			CHECK_RC_ASSERT(rc, EOK);

		}

	}

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
 * This function is handler for key deletion in B+ tree.
 */
int bplus_tree_tc_delete_handle(int i)
{

	int rc = EOK;
	char *file_name = NULL;
	b_plus_tree_key_t *key = NULL;
	bplus_tree_traverse_path_st *traverse_path = NULL;

	file_name = (char *)malloc(MAX_PATH);
	CHECK_RC_ASSERT((file_name == NULL), 0);

	key = (b_plus_tree_key_t *)malloc(KEY_SIZE);
	CHECK_RC_ASSERT((key == NULL), 0);
	memset(key, 0, KEY_SIZE);

	get_file_name(file_name, i);
	rc = bplus_form_key(file_name, key);
	CHECK_RC_ASSERT(rc, EOK);

	rc = bplus_tree_delete(ROOT, key);
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

	rc = EOK;
	free(file_name);
	free(key);

	return rc;

}

/*
 * This function deletes keys from bplus tree and searches the same key.
 */
int bplus_tc_delete_keys(bool is_ascending)
{

	int i, rc = EOK;

	/*
	 * Delete the inode no. of file into b+ tree.
	 * Then, lookup inside b+ tree for the key just deleted.
	 * Currently, testing only for single key deletion.
	 */
	if (is_ascending == TRUE)
	{

		for (i = 0; i < MAX_INPUT_ITEMS; i++)
		{

			rc = bplus_tree_tc_delete_handle(i);
			CHECK_RC_ASSERT(rc, EOK);

		}

	}
	else
	{

		for (i = MAX_INPUT_ITEMS - 1; i >= 0 ; i--)
		{

			rc = bplus_tree_tc_delete_handle(i);
			CHECK_RC_ASSERT(rc, EOK);

		}

	}

	return rc;

}

/*
 * This test case tests simple insertion/lookup/deletion of 16515 objects inside b+
 * tree.
 */
int bplus_test_case1(bool is_ascending_insert, bool is_ascending_delete)
{

	int rc = EOK;

	rc = bplus_create_data();
	CHECK_RC_ASSERT(rc, EOK);

	rc = bplus_tree_init(META_DIR, ROOT, TRUE);
	CHECK_RC_ASSERT(rc, EOK);

	/*
	 * Insert the inode no. of file into b+ tree.
	 */
	bplus_tc_insert_keys(is_ascending_insert);

	/*
	 * Now, search the items in b+ tree.
	 */
	bplus_tc_search_keys();

	/*
	 * Now, delete keys from B+ tree.
	 */
	rc = bplus_tc_delete_keys(is_ascending_delete);
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
	bool is_ascending_insert, is_ascending_delete;

	is_ascending_insert = TRUE;
	is_ascending_delete = TRUE;
	rc = bplus_test_case1(is_ascending_insert, is_ascending_delete);

	is_ascending_insert = FALSE;
	is_ascending_delete = TRUE;
	rc = bplus_test_case1(is_ascending_insert, is_ascending_delete);

	is_ascending_insert = TRUE;
	is_ascending_delete = FALSE;
	rc = bplus_test_case1(is_ascending_insert, is_ascending_delete);

	is_ascending_insert = FALSE;
	is_ascending_delete = FALSE;
	rc = bplus_test_case1(is_ascending_insert, is_ascending_delete);

	return rc;

}

