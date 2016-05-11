#include "b_plus_tree.h"

/*
 * This test case tests simple insertion/lookup of 8192 objects inside b+ tree.
 */
int bplus_test_case1()
{

	int rc = EOK;

	rc = bplus_tree_init(TRUE);
	CHECK_RC_ASSERT(rc, EOK);

	rc = bplus_tree_deinit();
	CHECK_RC_ASSERT(rc, EOK);

	return rc;

}

int bplus_tc_execute()
{

	int rc = EOK;

	rc = bplus_test_case1();

	return rc;

}

