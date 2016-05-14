#include "../include/header.h"
#define BPLUS_MAX_HEIGHT 5

/*
 * Following structure will remain on every node.
 */
typedef struct block_head
{

	uint16_t nr_items;
	uint16_t free_space;
	uint8_t level;
	uint8_t pad;

}block_head_st;

/*
 * Following structure represents on-disk 64 bit key. This will remain on internal
 * node of filesystem. path_hash will be computed from SHA-1 computation.
 */
typedef struct b_plus_tree_key
{

	ino_t i_ino;

}b_plus_tree_key_t;

/*
 * Following structure stores inode no. of file of next pointer.
 */
typedef struct disk_child
{

	ino_t i_ino;

}disk_child_st;

/*
 * Structure of internal node of the btree
 * n keys and n+1, disk children
 *
 * ------------------------------------------------------------------
 * |block | key1, key2,.... | disk child0, disk_child1, disk_child2 |
 * |head  |		    |					    |
 * |	  |		    |					    |
 * |	  |		    |					    |
 * ------------------------------------------------------------------
 */

/*
 * Following structure represents item stored on-disk.
 */
typedef struct item
{

	ino_t i_ino;

}item_st;

/* 
 * Typical structure for leaf block is as follow :
 * ----------------------------------------------------
 * |Block | item1,	|			|
 * |Head  | item2,	| 			|
 * -----------------------------------------------------
 */

typedef struct path_element
{

	/*
	 * In-memory contents of the file.
	 */
	void *pe_node;

	/*
	 * Position at which the key is located.
	 */
	int pe_position;

	/*
	 * path in meta directory.
	 */
	char *pe_path;

}path_element_st;

/*
 * Follwoing structure represents complete path of a key in b+ tree. It starts from 
 * root to the leaf block where key is located.
 */
typedef struct bplus_tree_traverse_path
{

	path_element_st path_elements[BPLUS_MAX_HEIGHT];

}bplus_tree_traverse_path_st;

