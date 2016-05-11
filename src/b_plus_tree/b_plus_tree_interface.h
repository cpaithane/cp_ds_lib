#include "../include/header.h"
#include "b_plus_tree.h"
#include <openssl/sha.h>

#define META_DIR ".meta"
#define ROOT ".meta/root"
#define NODE_SIZE 8192
#define OPEN_MODE 0744
#define OPEN_FLAGS (O_CREAT | O_TRUNC | O_RDWR)
#define BTREE_LEAF_LEVEL 0
#define BLOCK_HEAD_SIZE sizeof(block_head_t)

int bplus_tree_init(bool force_init);
int bplus_tree_format_node(char *file);
int bplus_tree_allocate_node(char *path, ino_t *i_ino);
int bplus_tree_deinit();

