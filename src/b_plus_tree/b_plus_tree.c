#include "b_plus_tree_interface.h"

/*
 * This function initializes b+ tree subsystem which involves following :
 * 1. Create meta directory. This directory stores all the internal and leaf nodes.
 * 2. Create and format root node of b+ tree.
 * 3. If meta directory is already present, then don't create it. I believe it is a
 *    request to reuse the existing b+ tree.
 * 4. Pass force_init flag, which indicates to delete and to create meta directory.
 * 5. Format meaning write a block_head data structure.
 */
int bplus_tree_init(bool force_init)
{

	int rc = EOK, rc1;
	struct stat *stat_buf;
	ino_t i_ino;

	stat_buf = (struct stat*)malloc(sizeof(struct stat));
	CHECK_RC_ASSERT((stat_buf == NULL), 0);

	if (stat(META_DIR, stat_buf) == -1)
	{
		rc = errno;
	}

	free(stat_buf);

	/*
	 * If stat yields success and force_init is true, then delete the .meta.
	 */
	if (rc == EOK)
	{

		if (force_init == TRUE)
		{

			rc1 = bplus_tree_deinit();
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
	rc = mkdir(META_DIR, 0744);
	CHECK_RC_ASSERT(rc, EOK);

	/*
	 * Now, create root node of the plus tree under .meta.
	 */
	rc = bplus_tree_allocate_node(ROOT, &i_ino);
	CHECK_RC_ASSERT(rc, EOK);

	return rc;

}

/*
 * This function removes meta directory.
 */
int bplus_tree_deinit()
{

	int rc = EOK;
	char cmd[20] = {0};
	
	snprintf(cmd, 20, "rm -rf %s", META_DIR);
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
	int fd = -1;

	CHECK_RC_ASSERT((i_ino == NULL), 0);
	CHECK_RC_ASSERT((path == NULL), 0);

	*i_ino = 0;
	fd = open(path, OPEN_FLAGS, OPEN_MODE);
	if (fd == -1)
	{

		rc = errno;
		return rc;

	}

	stat_buf = (struct stat*)malloc(sizeof(struct stat));
	CHECK_RC_ASSERT((stat_buf == NULL), 0);

	if (stat(path, stat_buf) == -1)
	{

		rc = errno;
		free(stat_buf);
		return rc;

	}

	*i_ino = stat_buf->st_ino;
	free(stat_buf);

	rc = bplus_tree_format_node(path);
	CHECK_RC_ASSERT(rc, EOK);	
	close(fd);

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
	int fd = -1;
	block_head_t *block_head = NULL;
	int bytes_written;

	fd = open(path, OPEN_FLAGS, OPEN_MODE);
	if (fd == -1)
	{

		rc = errno;
		return rc;

	}

	block_head = (block_head_t *)malloc(BLOCK_HEAD_SIZE);
	memset(block_head, 0, BLOCK_HEAD_SIZE);
	block_head->nr_items = 0;
	block_head->free_space = NODE_SIZE - BLOCK_HEAD_SIZE;
	block_head->level = BTREE_LEAF_LEVEL;

	bytes_written = write(fd, block_head, BLOCK_HEAD_SIZE);
	if (bytes_written != BLOCK_HEAD_SIZE)
	{
		rc = errno;
	}

	free(block_head);
	close(fd);
	return rc;

}

