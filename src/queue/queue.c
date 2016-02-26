#include "queue.h"

/*
 * This function checks following : 
 * If rear end is pointing at maximum size, then check front end of queue.
 * Scenario #1: If front end is at -1 then queue is full. EOVERFLOW is returned.
 *
 * Scenario #2: Else queue is not full. In this scenario, items should be shifted to
 * front end decremented to accomodate new item inside queue to make front point to 
 * -1. ENOTEMPTY is returned.
 */
int queue_is_queue_full(queue_st *q)
{

	int rc = EOK;

	if (q->q_rear < (QUEUE_MAX_ITEMS - 1))
	{ 
		return rc;
	}

	/*
	 * Scenario #1
	 */
	if (q->q_front == (QUEUE_MIN_ITEMS + 1))
	{
		rc = EOVERFLOW;
	}
	/*
	 * Scenario #2
	 */
	else
	{
		rc = ENOTEMPTY;
	}

	return rc;

}

/*
 * This function checks whether queue is empty or not.
 * If front end = rear end + 1, then queue is empty.
 */
int queue_is_queue_empty(queue_st *q)
{

	int rc = ENOTEMPTY;

	if ((q->q_front) == (q->q_rear + 1) || 
            ((q->q_front == QUEUE_MIN_ITEMS) && 
            (q->q_rear == QUEUE_MIN_ITEMS)))
	{
		rc = EOK;
	}

	return rc;

}

/*
 * This function mallocs memory of len bytes
 */
void *queue_alloc_memory(size_t len)
{

	void *node = NULL;

	node = malloc(len);
	memset(node, 0, len);
	return node;

}

/*
 * This function allocates queue and nodes inside queue.
 */
queue_st* queue_alloc_queue(size_t len)
{

	int i;
	queue_st *q = NULL;

	q = queue_alloc_memory(sizeof(queue_st));

	for (i = 0; i < QUEUE_MAX_ITEMS; i++)
	{
		q->q_node[i] = queue_alloc_memory(len);
	}

	queue_initialize(q);
	return q;

}

/*
 * This function deallocates every node and queue structure.
 */
int queue_dealloc_queue(queue_st *q)
{

	int rc = EOK;
	int i;

	rc = queue_is_queue_empty(q);
	CHECK_RC(rc, EOK);

	for (i = 0; i < QUEUE_MAX_ITEMS; i++)
	{
		free(q->q_node[i]);
	}

	free(q);

	return rc;

}

/*
 * This function initializes queue's front and rear to -1
 */
void queue_initialize(queue_st *q)
{

	q->q_rear = QUEUE_MIN_ITEMS;
	q->q_front = QUEUE_MIN_ITEMS;

}

/*
 * This function handles scenario #2 mentioned in queue_is_queue_full()
 */
int queue_compact_queue(queue_st *q, size_t len)
{

	int i, start, end, num_nodes;
	void *node_from = NULL;
	void *node_to = NULL;

	/*
	 * Compaction should run only if q_rear is pointing at max.
	 */
	CHECK_RC_ASSERT(q->q_rear, (QUEUE_MAX_ITEMS - 1));

	/*
	 * It is a bug if front is pointing at -1 or 0.
	 */
	CHECK_RC_ASSERT(q->q_front == QUEUE_MIN_ITEMS, 0);
	CHECK_RC_ASSERT(q->q_front == (QUEUE_MIN_ITEMS + 1), 0);

	num_nodes = q->q_rear - q->q_front + 1;
	start = q->q_front;

	for (i = 0; i < num_nodes; i++, start++)
	{

		node_from = q->q_node[start];
		node_to = q->q_node[i];
		memcpy(node_to, node_from, len);

	}

	/*
	 * front = 0, rear = num_nodes - 1
	 */
	(q->q_front) = (QUEUE_MIN_ITEMS + 1);
	(q->q_rear) = num_nodes - 1;
	CHECK_RC_ASSERT(q->q_front, (QUEUE_MIN_ITEMS + 1));
	CHECK_RC_ASSERT(q->q_rear == QUEUE_MAX_ITEMS, 0);
	return EOK;

}

/*
 * This function inserts data into queue at rear end.
 */
int queue_eneque(queue_st *q, void *data, size_t len)
{

	int rc = EOK;

	rc = queue_is_queue_full(q);
	/*
	 * If queue is getting overflown, then don't insert new node.
	 */
	if (rc == EOVERFLOW)
	{
		return rc;
	}
	/*
	 * If queue is not empty, then run compaction.
	 */
	else if (rc == ENOTEMPTY)
	{

		rc = queue_compact_queue(q, len);
		CHECK_RC(rc, EOK);

	}
	else
	{
		CHECK_RC_ASSERT(rc, EOK);
	}

	/*
	 * Now, insert item at rear end of queue.
	 */
	if (q->q_front == QUEUE_MIN_ITEMS)
	{
		q->q_front = 0;
	}

	(q->q_rear)++;
	memcpy(q->q_node[q->q_rear], data, len);

	return rc;

}

/*
 * This function dequeues a node pointed by front end of queue.
 */
void *queue_deque(queue_st *q, int *status)
{

	int rc = EOK;
	void *node = NULL;

	/*
	 * Check whether queue is not empty.
	 */
	rc = queue_is_queue_empty(q);
	if (rc == EOK)
	{

		*status = ENOENT;
		return node;

	}

	CHECK_RC_ASSERT(rc, ENOTEMPTY);
	node = q->q_node[q->q_front];
	(q->q_front)++;
	return node;

}

/*
 * This function prints nodes pointed by rear end and ends with front end.
 */
int queue_list(queue_st *q)
{

	int rc = EOK;
	int i;
	int *node = NULL;

	rc = queue_is_queue_empty(q);
	CHECK_RC(rc, ENOTEMPTY);
	rc = EOK;

	printf("\n");
	for (i = q->q_front; i < (q->q_rear + 1); i++)
	{

		node = (int *)(q->q_node[i]);
		printf("%d\n", *node);

	}

	printf("\n");
	return rc;

}

