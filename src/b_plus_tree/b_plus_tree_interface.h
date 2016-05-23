#include "../include/header.h"
#include "b_plus_tree.h"

#define META_DIR ".meta\0"
#define ROOT ".meta/root\0"
#define ALLOCATOR ".meta/allocator\0"

#define OPEN_MODE 0744
#define READ_MODE 0444
#define WRITE_MODE 0744

#define OPEN_FLAGS (O_CREAT | O_TRUNC | O_RDWR)
#define READ_FLAGS (O_RDONLY)
#define WRITE_FLAGS (O_WRONLY)

#define BTREE_LEAF_LEVEL 0
#define BLOCK_HEAD_SIZE sizeof(block_head_st)
#define NODE_SIZE (BLOCK_HEAD_SIZE + 1024)
#define KEY_SIZE sizeof(b_plus_tree_key_t)
#define TRAVERSE_PATH_SIZE sizeof(bplus_tree_traverse_path_st)
#define PE_SIZE sizeof(path_element_st)
#define DC_SIZE sizeof(disk_child_st)
#define ITEM_SIZE sizeof(item_st)
#define TB_SIZE sizeof(bplus_tree_balance_st)

#define MAX_ITEMS ((NODE_SIZE - BLOCK_HEAD_SIZE) / ITEM_SIZE)
#define MAX_KEYS ((NODE_SIZE - BLOCK_HEAD_SIZE - DC_SIZE)) / (DC_SIZE + KEY_SIZE)
#define MAX_DC (MAX_KEYS + 1)
#define NR_KEYS(nr_items) ( (nr_items - 1) / 2 )

#define INVALID_PATH_LENGTH -1
#define PATH_POSITION_ROOT 1

int bplus_tree_init(char *meta_dir, char *root_path, bool force_init);
int bplus_tree_write_root_path(char *root_path, ino_t root_ino);
int bplus_tree_format_node(char *file);
int bplus_tree_get_new_node(ino_t *i_ino);
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
int bplus_tree_punch_key(void *buf, b_plus_tree_key_t *key, uint16_t position);
int bplus_tree_punch_dc(void *buf, disk_child_st *dc, uint16_t position);
int bplus_tree_punch_item(void *buf, item_st *item, uint16_t position);

int bplus_tree_get_neighbor(
		bplus_tree_traverse_path_st *traverse_path,
		uint8_t level, void *neighbor, neighbor_et type, char *path);

int bplus_tree_first_dc_pos(void *buf);
void bplus_tree_fill_pe(void *node,
			char *path,
			int position,
			path_element_st *pe);

void bplus_tree_copy_pe(bplus_tree_traverse_path_st *traverse_path,
                        void *buf,
                        uint8_t level,
                        int position,
                        char *path);

item_st *bplus_tree_get_item(void *buf, uint16_t position);

b_plus_tree_key_t *bplus_tree_get_rkey(
                        bplus_tree_traverse_path_st *traverse_path,
                        uint8_t level);
b_plus_tree_key_t *bplus_tree_get_lkey(
                        bplus_tree_traverse_path_st *traverse_path,
                        uint8_t level);
b_plus_tree_key_t *bplus_tree_get_key(void *buf, uint16_t position);
void bplus_tree_form_item(b_plus_tree_key_t *key, item_st *item);

int bplus_key_compare(const void *key1, const void *key2);
int bin_search(
        void *buf,
        b_plus_tree_key_t *key,
	int16_t nr_items,
        int *position,
        bool is_leaf);

int bplus_tree_flush_siblings(sll_st *tb_left);
int bplus_tree_flush_tb(bplus_tree_balance_st *tb);
int bplus_tree_flush_traverse_path(bplus_tree_traverse_path_st *traverse_path);
int bplus_tree_free_traverse_path(bplus_tree_traverse_path_st *traverse_path);

int bplus_tree_shift_right_keys(void *internal_node,
                                b_plus_tree_key_t *key,
                                uint16_t nr_keys);

int bplus_tree_shift_right_dc(void *internal_node, int position, uint16_t nr_dc);

int bplus_tree_shift_left(void *leaf_node, int position, uint16_t nr_items);
int bplus_tree_shift_right(void *leaf_node, item_st *item, uint16_t nr_items);

void *bplus_tree_get_node_path(
		bplus_tree_traverse_path_st *traverse_path,
		uint8_t level);
void *bplus_tree_get_parent_node_path(
		bplus_tree_traverse_path_st *traverse_path,
		uint8_t level);
int bplus_tree_get_pos_path(bplus_tree_traverse_path_st *traverse_path, 
                            uint8_t level);

char *bplus_tree_get_pe_path_path(bplus_tree_traverse_path_st *traverse_path, 
                            uint8_t level);

int bplus_tree_flow_item(void *src, void *dest);
int bplus_tree_rebalance(char *root_path,
			 bplus_tree_traverse_path_st *traverse_path,
                         item_st *item,
                         rebalnce_mode_et mode);
int bplus_tree_rebalance_insert(char *root_path,
				bplus_tree_traverse_path_st *traverse_path,
                                item_st *item);
int bplus_tree_rebalance_delete(bplus_tree_traverse_path_st *traverse_path,
                                item_st *item);

int bplus_tree_initialize_allocator();

bplus_tree_balance_st *bplus_tree_init_tb(
			char *root_path,
			bplus_tree_traverse_path_st *traverse_path);
int bplus_tree_rebalance_leaf_insert(bplus_tree_balance_st *tb, item_st *item);
int bplus_tree_rebalance_insert_handle(bplus_tree_balance_st *tb,
				       item_st *item);
int bplus_tree_rebalance_internal_insert(bplus_tree_balance_st *tb,
                                         item_st *item, uint8_t level);

int bplus_tree_flow_item_handle(bplus_tree_balance_st *tb,
				item_st *item,
				ino_t new_leaf_ino,
				bool flow_mode);

int bplus_tree_flow_item_case1(
                            bplus_tree_balance_st *tb,
                            bool flow_mode,
                            void *leaf_node,
                            void *new_leaf_node,
                            void *new_internal_node,
                            item_st *item,
                            char *internal_node_path,
                            char *leaf_node_path,
			    ino_t new_leaf_ino,
			    ino_t new_internal_ino);

bool bplus_tree_find_flow_dir(int pe_position);

int bplus_tree_adjust_root(char *root_path, ino_t new_root_ino);

void bplus_tree_init_internal_node(void *new_internal_node,
                                   b_plus_tree_key_t *key,
                                   disk_child_st *dc0,
                                   disk_child_st *dc1);

void bplus_tree_adjust_internal(void *internal_node,
                                b_plus_tree_key_t *key,
                                disk_child_st *dc);

