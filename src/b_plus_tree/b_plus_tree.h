#include "../include/header.h"

/*
 * Following structure will remain on every node.
 */
typedef struct block_head
{

	uint16_t nr_items;
	uint16_t free_space;
	uint8_t level;
	uint8_t pad;

}block_head_t;

/*
 * Following structure represents on-disk 64 bit key. This will remain on internal
 * node of filesystem. path_hash will be computed from SHA-1 computation.
 */
typedef struct b_plus_tree_key
{

	uint32_t path_hash;
	uint32_t gen_num;

}b_plus_tree_key_t;

/*
 * Following structure stores inode no. of file of next pointer.
 */
struct disk_child
{

	ino_t inode_no;

}disk_child_t;

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
 * Following structure represents metadata for item. It will only occupy leaf node.
 */
typedef struct item_head
{

	b_plus_tree_key_t ih_key;
	uint32_t item_location;

}item_head_t;

/*
 * Following structure represents item stored on-disk.
 */
typedef struct item
{

	ino_t inode_no;

}item_t;

/* 
 * Typical structure for leaf block is as follow :
 * ----------------------------------------------------
 * |Block | item_head1,		| item2,	|
 * |Head  | item_head2,  	| item1.	|
 * |	  | 			| 		|
 * -----------------------------------------------------
 */

