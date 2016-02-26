#include "queue.h"

/*
 * This test case tests basic functionality of queue.
 */
int queue_test_case1()
{

	int rc = EOK;
	int i;
	queue_st *q = NULL;

	q = queue_alloc_queue(sizeof(int));

	/*
	 * Insert into queue unless it is full and further insertion should result 
	 * into EOVERFLOW.
	 */
	for (i = 0; i < QUEUE_MAX_ITEMS; i++)
	{

		rc = queue_eneque(q, &i, sizeof(int));
		CHECK_RC_ASSERT(rc, EOK);

	}

	rc = queue_eneque(q, &i, sizeof(int));
	CHECK_RC_ASSERT(rc, EOVERFLOW);

	rc = queue_list(q);
	CHECK_RC_ASSERT(rc, EOK);

	for (i = 0; i < QUEUE_MAX_ITEMS; i++)
	{

		queue_deque(q, &rc);
		CHECK_RC_ASSERT(rc, EOK);

	}

	queue_deque(q, &rc);
	CHECK_RC_ASSERT(rc, ENOENT);

	rc = queue_list(q);
	CHECK_RC_ASSERT(rc, EOK);

	rc = queue_dealloc_queue(q);
	CHECK_RC_ASSERT(rc, EOK);

	return rc;

}

/*
 * This test case executes compaction.
 */
int queue_test_case2()
{

	int rc = EOK;
	int i;
	queue_st *q = NULL;

	q = queue_alloc_queue(sizeof(int));

	/*
	 * Insert into queue unless it is full and further insertion should result 
	 * into EOVERFLOW.
	 */
	for (i = 0; i < QUEUE_MAX_ITEMS; i++)
	{

		rc = queue_eneque(q, &i, sizeof(int));
		CHECK_RC_ASSERT(rc, EOK);

	}

	rc = queue_eneque(q, &i, sizeof(int));
	CHECK_RC_ASSERT(rc, EOVERFLOW);

	rc = queue_list(q);
	CHECK_RC_ASSERT(rc, EOK);

	/*
	 * Remove couple of items from queue and eneque again to execute compaction.
	 */

	for (i = 0; i < (QUEUE_MAX_ITEMS - 2); i++)
	{

		queue_deque(q, &rc);
		CHECK_RC_ASSERT(rc, EOK);

	}

	rc = queue_list(q);
	CHECK_RC_ASSERT(rc, EOK);

	rc = queue_eneque(q, &i, sizeof(int));
	CHECK_RC_ASSERT(rc, EOK);

	rc = queue_eneque(q, &i, sizeof(int));
	CHECK_RC_ASSERT(rc, EOK);

	rc = queue_list(q);
	CHECK_RC_ASSERT(rc, EOK);

	for (i = 0; i < (QUEUE_MAX_ITEMS); i++)
	{
		queue_deque(q, &rc);
	}

	rc = queue_dealloc_queue(q);
	CHECK_RC_ASSERT(rc, EOK);

	return rc;

}

int main(int argc, char **argv)
{

	int rc = EOK;

	rc = queue_test_case1();
	rc = queue_test_case2();

	return rc;

}

