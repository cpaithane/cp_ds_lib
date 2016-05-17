#include "../include/header.h"

/*
 * This function compares integer data and returns 
 * 0 if both are equal
 * 1 if data1 > data2
 * -1 if data1 < data2
 */
int common_int_data_compare(const void *data1, const void *data2)
{

	int ret_val = IDENTICAL;
	int val1 = *((int *)data1);
	int val2 = *((int *)data2);

	if (val1 < val2)
	{
		ret_val = FIRST_LESS;
	}
	else if (val1 > val2)
	{
		ret_val = FIRST_GREATER;
	}
	
	return ret_val;

}

/*
 * This function swaps data pointers.
 */
void swap(void *data1, void *data2)
{

	void *tmp = data1;
	data1 = data2;
	data2 = tmp;

}

/*
 * This function finds out greatest from three elements.
 */
void *find_greatest_from_three(
			    void *root,
			    void * left_child,
			    void *right_child,
			    common_data_compare_t compare)
{

	int rc;
	void *greatest = left_child;

	rc = compare(root, left_child);
	if (rc == FIRST_GREATER)
	{
		greatest = root;
	}

	rc = compare(greatest, right_child);
	if (rc == FIRST_LESS)
	{
		greatest = right_child;
	}

	return greatest;

}

/*
 * This function finds out smallest from three elements.
 */
void *find_smallest_from_three(
			    void *root,
			    void * left_child,
			    void *right_child,
			    common_data_compare_t compare)
{

	int rc;
	void *smallest = left_child;

	rc = compare(root, left_child);
	if (rc == FIRST_LESS)
	{
		smallest = root;
	}

	rc = compare(smallest, right_child);
	if (rc == FIRST_GREATER)
	{
		smallest = right_child;
	}

	return smallest;

}

/*
 * This function does a stat for path. If stat returns ENOENT, then path is absent. 
 */
int is_path_present(char *path, ino_t *i_ino)
{

	int rc = EOK;
	struct stat *stat_buf;

	CHECK_RC_ASSERT((path == NULL), 0);
	CHECK_RC_ASSERT((i_ino == NULL), 0);

	stat_buf = (struct stat*)malloc(sizeof(struct stat));
	CHECK_RC_ASSERT((stat_buf == NULL), 0);

	if (stat(path, stat_buf) == -1)
	{

		rc = errno;
		*i_ino = 0;

	}
	else
	{
		*i_ino = stat_buf->st_ino;
	}

	free(stat_buf);
	return rc;

}

/*
 * This function does following things : 
 * 1. Checks whether the path is present.
 * 2. Reads entire contents of file and returns buf to caller.
 */
int read_file_contents(
		char *path,
		void *buf,
		int read_flags,
		mode_t mode,
		size_t len)
{

	int rc = EOK;
	ino_t i_ino;
	int fd = -1;
	int bytes_read;

	CHECK_RC_ASSERT((path == NULL), 0);
	CHECK_RC_ASSERT((buf == NULL), 0);

	rc = is_path_present(path, &i_ino);
	if (rc != EOK)
	{
		return rc;
	}

	fd = open(path, read_flags, mode);
	if (fd == -1)
	{

		rc = errno;
		return rc;

	}

	memset(buf, 0, len);
	bytes_read = read(fd, buf, len);
	if (bytes_read != len)
	{
		rc = errno;
	}

	close(fd);
	return rc;

}

/*
 * This function issues readdir on parent_dir and finds out matching inode no.
 * It returns d_name of matching directory entry.
 */
int get_path(char *parent_dir, ino_t i_ino, char *path)
{

	int rc = EOK;
	struct dirent *dirent = NULL;
	DIR *dirp = NULL;
	int bytes_copied;

	CHECK_RC_ASSERT((path == NULL), 0);
	CHECK_RC_ASSERT((parent_dir == NULL), 0);

	dirp = opendir(parent_dir);
	CHECK_RC_ASSERT((dirp == NULL), 0);

	while (1)
	{

		dirent = readdir(dirp);
		if ((dirent == NULL) && (errno != EOK))
		{

			rc = errno;
			break;

		}

		if (dirent->d_ino == i_ino)
		{

			memset(path, 0, MAX_PATH);
			snprintf(path, MAX_PATH, "%s/%s",
				 parent_dir, dirent->d_name);
			bytes_copied = strlen(dirent->d_name) + 
					strlen(parent_dir) + 1;
			path[bytes_copied] = '\0';
			break;

		}

	}

	closedir(dirp);
	return rc;

}

/*
 * This is a generic write function.
 */
int write_file_contents(char *path,
			int flags,
			mode_t mode,
			void *buf,
			size_t len)
{

	int fd;
	int bytes_written;
	int rc = EOK;

	fd = open(path, flags, mode);
	if (fd == -1)
	{

		rc = errno;
		return rc;

	}

	bytes_written = write(fd, buf, len);
	if (bytes_written != len)
	{
		rc = errno;
	}

	close(fd);
	return rc;

}

