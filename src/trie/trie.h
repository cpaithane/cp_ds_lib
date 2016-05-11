#include "../include/header.h"

#define TRIE_MAX_POINTERS 26

/*
 * Data structure for a node of trie. As test case of a-z is being targeted, will
 * keep 26 pointers inside a node.
 */
typedef struct trie_node
{

	char trie_node_value;
	struct trie_node *trie_node_pointers[TRIE_MAX_POINTERS];

}trie_node_st;

/*
 * Data structure for trie. It is an array of trie_node_st.
 */
typedef struct trie
{

	trie_node_st *root;
	uint32_t node_count;

}trie_st;

/*
 * Following APIs are used for manipulation of trie.
 */
trie_st *trie_initialize();
int trie_deinitialize(trie_st *trie);
trie_node_st *trie_allocate_node(char ch);
int trie_deallocate_node(trie_node_st *node);
int trie_insert(trie_st *trie, char *key);
int trie_search(trie_st *trie, char *key, int *num_comp);
int trie_delete(trie_node_st *trie, char *key, int level, int len);

