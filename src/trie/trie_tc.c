#include "trie.h"

/*
 * This test case inserts multiple elements inside trie and searches for it. It also
 * prints no. of comparisons made while searching.
 */
int trie_test_case1()
{

	int rc = EOK;
	int i, num_comp;
	trie_st *trie = NULL;
	char keys[][7] = {"the", "any", "there", "answer", "by", "bye", "the"};
	char keys_undup[][7] = {"there", "answer", "bye"};

	trie = trie_initialize();
	CHECK_RC_ASSERT((trie == NULL), 0);

	for (i = 0; i < 7; i++)
	{

		rc = trie_insert(trie, keys[i]);
		CHECK_RC_ASSERT(rc, EOK);

	}

	for (i = 0; i < 7; i++)
	{

		rc = trie_search(trie, keys[i], &num_comp);
		printf("Trie search hit %d comparisons for %s\n", num_comp, keys[i]);
		CHECK_RC_ASSERT(rc, EOK);
		CHECK_RC_ASSERT(num_comp, strlen(keys[i]));

	}

	rc = trie_search(trie, "chetan", &num_comp);
	CHECK_RC_ASSERT(rc, ENOENT);
	rc = EOK;

	for (i = 0; i < 3; i++)
	{

		rc = trie_delete(trie->root, keys_undup[i], 0, strlen(keys_undup[i]));
		CHECK_RC_ASSERT(rc, EOK);

	}

	return rc;

}

/*
 * This is workhorse function for test cases.
 */
int trie_tc_execute()
{

	int rc = EOK;

	rc = trie_test_case1();

	return rc;

}
