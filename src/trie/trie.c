#include "trie.h"

/*
 * It is preferred that allocation of any type memory for trie node is deferred 
 * unless actual request comes. This way, memory will be utilized in best manner
 * and unnecessary allocations will not be done.
 */

/*
 * This function initializes root node of trie.
 */
trie_st *trie_initialize()
{

	trie_st *trie = (trie_st*)malloc(sizeof(trie_st));
	if (trie == NULL)
	{
		return NULL;
	}

	trie->node_count = 0;
	trie->root = trie_allocate_node('0');
	return trie;

}

/*
 * This function deinitializes the root node of trie.
 */
int trie_deinitialize(trie_st *trie)
{

	int rc = EOK;
	CHECK_RC_ASSERT((trie == NULL), 0);

/*
	if (trie->node_count != 0)
	{
		rc = trie_deallocate_all_nodes(trie);
	}

	CHECK_RC_ASSERT((trie->node_count, 0);
*/
	free(trie);
	return rc;

}

/*
 * This function allocates a node of a trie.
 */
trie_node_st *trie_allocate_node(char ch)
{

	int i;
	trie_node_st *node = NULL;

	node = malloc(sizeof(trie_node_st));
	if (node == NULL)
	{
		return NULL;
	}

	node->trie_node_value = ch;

	for (i = 0; i < TRIE_MAX_POINTERS; i++)
	{
		node->trie_node_pointers[i] = NULL;
	}

	return node;

}

/*
 * This function deallocates memory for node.
 */
int trie_deallocate_node(trie_node_st *node)
{

	int rc = EOK;

	CHECK_RC_ASSERT((node == NULL), 0);
	free(node);

	return rc;

}

/*
 * This function inserts key in trie. Insertion goes down level by level.
 */
int trie_insert(trie_st *trie, char *key)
{

	int rc = EOK;
	int i, len, start_idx;
	trie_node_st *tmp_node;

	CHECK_RC_ASSERT((key == NULL), 0);
	CHECK_RC_ASSERT((trie == NULL), 0);

	len = strlen(key);
	tmp_node = trie->root;

	for (i = 0; i < len; i++)
	{

		start_idx = key[i] - 'a';
		if (tmp_node->trie_node_pointers[start_idx] == NULL)
		{

			tmp_node->trie_node_pointers[start_idx] = 
				trie_allocate_node(key[i]);

		}

		tmp_node = tmp_node->trie_node_pointers[start_idx];

	}

	(trie->node_count)++;
	return rc;

}

/*
 * This function goes through trie and searches for key. It also returns number of
 * comparisons done. This should be length of key.
 */
int trie_search(trie_st *trie, char *key, int *num_comp)
{

	int rc = EOK;
	int len, i, start_idx;
	*num_comp = 0;

	CHECK_RC_ASSERT((key == NULL), 0);

	trie_node_st *tmp_node = trie->root;
	len = strlen(key);

	for (i = 0; i < len; i++)
	{

		start_idx = key[i] - 'a';
		if (tmp_node->trie_node_pointers[start_idx] == NULL)
		{

			rc = ENOENT;
			break;

		}
		(*num_comp)++;
		tmp_node = tmp_node->trie_node_pointers[start_idx];

	}

	return rc;

}

/*
 * Following function deletes a branch pointed by key.
 */
int trie_delete(trie_node_st *node, char *key, int level, int len)
{

	int rc = EOK;
	int start_idx = key[level] - 'a';

	if (len != level)
	{
		trie_delete(node->trie_node_pointers[start_idx], key, level + 1, len);
	}

	free(node->trie_node_pointers[start_idx]);
	return rc;

}

