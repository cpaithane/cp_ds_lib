#include "../include/header.h"
#include "b_plus_tree.h"

#define META_DIR ".meta\0"
#define ROOT ".meta/root\0"

#define OPEN_MODE 0744
#define READ_MODE 0444
#define WRITE_MODE 0744

#define OPEN_FLAGS (O_CREAT | O_TRUNC | O_RDWR)
#define READ_FLAGS (O_RDONLY)
#define WRITE_FLAGS (O_WRONLY)

#define BTREE_LEAF_LEVEL 0
#define BLOCK_HEAD_SIZE sizeof(block_head_st)
#define NODE_SIZE (BLOCK_HEAD_SIZE + 8192)
#define KEY_SIZE sizeof(b_plus_tree_key_t)
#define TRAVERSE_PATH_SIZE sizeof(bplus_tree_traverse_path_st)
#define DC_SIZE sizeof(disk_child_st)
#define ITEM_SIZE sizeof(item_st)
#define MAX_ITEMS ((NODE_SIZE - BLOCK_HEAD_SIZE) / ITEM_SIZE)

int bplus_tree_init(char *meta_dir, char *root_path, bool force_init);
int bplus_tree_format_node(char *file);
int bplus_tree_allocate_node(char *path, ino_t *i_ino);
int bplus_tree_deinit(char *meta_dir);

int bplus_tree_delete_item(bplus_tree_traverse_path_st *traverse_path);
int bplus_tree_delete(char *root_path, char *path);

int bplus_tree_insert(char *root_path, char *path);
int bplus_form_key(char *path, b_plus_tree_key_t *key);

int bplus_tree_search_key(char *root_path,
                          b_plus_tree_key_t *key,
                          bplus_tree_traverse_path_st *traverse_path);

block_head_st* bplus_tree_get_block_head(void *buf);
disk_child_st *bplus_tree_get_dc(void *buf, int position);
int bplus_tree_first_dc_pos(void *buf);
void bplus_tree_copy_pe(bplus_tree_traverse_path_st *traverse_path,
                        void *buf,
                        uint8_t level,
                        int position,
                        char *path);
item_st *bplus_tree_get_item(void *buf, uint16_t position);
b_plus_tree_key_t *bplus_tree_get_key(void *buf, uint16_t position);
void bplus_tree_form_item(b_plus_tree_key_t *key, item_st *item);

int bplus_key_compare(const void *key1, const void *key2);
int bin_search(
        void *buf,
        b_plus_tree_key_t *key,
	uint16_t nr_items,
        int *position,
        bool is_leaf);

int bplus_tree_flush_traverse_path(bplus_tree_traverse_path_st *traverse_path);
int bplus_tree_free_traverse_path(bplus_tree_traverse_path_st *traverse_path);

int bplus_tree_shift_left(void *leaf_node, int position, uint16_t nr_items);
int bplus_tree_shift_right(void *leaf_node, item_st *item, uint16_t nr_items);

void *bplus_tree_get_leaf_path(bplus_tree_traverse_path_st *traverse_path);
int bplus_tree_get_pos_path(bplus_tree_traverse_path_st *traverse_path, 
                            uint8_t level);

