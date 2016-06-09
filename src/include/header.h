#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>

#define MAX_PATH 256
#define TRUE 1
#define FALSE 0

#define EOK 0
#define MAX_INT ((unsigned int)(-1))
#define FIRST_LESS -1
#define FIRST_GREATER 1
#define IDENTICAL 0

#define MAX(a, b) ((a > b) ? (a) : (b))

#define CHECK_RC(actual_rc, expected_rc)\
do {\
	\
	if (actual_rc != expected_rc)	\
		return actual_rc;	\
	\
}while(0)

#define CHECK_RC_ASSERT(actual_rc, expected_rc)\
do {\
	\
	if (actual_rc != expected_rc)	\
		assert(0);	\
	\
}while(0)

#define CHECK_BOOL_RET_RC(actual_bool, expected_bool, rc)\
do {\
	\
	if (actual_bool != expected_bool)	\
		return rc;	\
	\
}while(0)

/*
 * Common APIs to be used.
 */
typedef int (*common_data_compare_t)(const void *, const void *);
int common_int_data_compare(const void *data1, const void *data2);
void swap(void *data1, void *data2);
void *find_greatest_from_three(
			    void *root,
			    void * left_child,
			    void *right_child,
			    common_data_compare_t compare);

void *find_smallest_from_three(
			    void *root,
			    void * left_child,
			    void *right_child,
			    common_data_compare_t compare);

int is_path_present(char *path, ino_t *i_ino);

int read_file_contents(
                char *path,
                void *buf,
                int read_flags,
                mode_t mode,
                size_t len);

int write_file_contents(char *path,
			int flags,
			mode_t mode,
			void *buf,
			size_t len);

int delete_file(char *path);

int get_path(char *parent_dir, ino_t i_ino, char *path);


/*
 * Following functions are entry points for test cases.
 */
int stack_tc_execute();
int queue_tc_execute();
int bst_tc_execute();
int sll_tc_execute();
int dll_tc_execute();
int scll_tc_execute();
int heap_tc_execute();
int graph_tc_execute();
int trie_tc_execute();
int bplus_tc_execute();
int rb_tree_tc_execute();

