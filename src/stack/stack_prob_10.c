/*
 * Description : Implement stack using queues.
 */

#include "stack.h"
#include "../queue/queue.h"

/*
 * This function pushes data in q1.
 */
int stack_prob_10_push(queue_st *q1, queue_st *q2, void *data, size_t size)
{

	int rc = EOK;
	rc = queue_eneque(q1, data, size);
	return rc;

}

/*
 * This function does all the dirty work for pop. Algorithm is stated in function.
 */
void* stack_prob_10_pop(queue_st **q1_ptr, queue_st **q2_ptr, size_t size, int *rc)
{

	void *data = NULL;
	*rc = ENOENT;
	queue_st *tmp;
	queue_st *q1 = *q1_ptr;	
	queue_st *q2 = *q2_ptr;	

	/*
	 * If q1 is empty, then it is an error.
	 */
	if (queue_is_queue_empty(q1) != ENOTEMPTY)
	{

		*rc = ENOENT;
		return data;

	}

	/*
	 * Deque from q1 till single element in q1 remains.
	 * Eneque the same element into q2.
	 */
	while (q1->q_front != q1->q_rear)
	{

		data = queue_deque(q1, rc);
		CHECK_RC_ASSERT((data == NULL), 0);

		*rc = queue_eneque(q2, data, size);
		CHECK_RC_ASSERT(*rc, EOK);
		

	}

	data = queue_deque(q1, rc);
	CHECK_RC_ASSERT((data == NULL), 0);

	/*
	 * Now, make q2 as q1.
	 */
	tmp = *q1_ptr;
	*q1_ptr = *q2_ptr;
	*q2_ptr = tmp;

	*rc = EOK;
	return data;

}

/*
 * This function is workhorse for problem defined in description secton. Idea is to
 * make pop operation heavy.
 */
int stack_prob_10()
{

	int i, rc = EOK;
	int *data;
	queue_st *q1, *q2;

	q1 = queue_alloc_queue(sizeof(int));
	q2 = queue_alloc_queue(sizeof(int));

	for (i = 0; i < 10; i++)
	{

		printf("Pushing %d on stack \n", i);
		stack_prob_10_push(q1, q2, &i, sizeof(int));

	}


	for (i = 0; i < 10; i++)
	{

		data = (int*)stack_prob_10_pop(&q1, &q2, sizeof(int), &rc);
		printf("Popping %d from stack \n", *data);

	}
	
	rc = queue_dealloc_queue(q1);
	rc = queue_dealloc_queue(q2);
	return rc;

}
