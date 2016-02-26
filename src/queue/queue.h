#include "../include/header.h"

#define QUEUE_MAX_ITEMS 5
#define QUEUE_MIN_ITEMS -1

/*
 * Data structure for queue.
 */
typedef struct queue{

	int q_rear;
	int q_front;
	void *q_node[QUEUE_MAX_ITEMS];

}queue_st;

/*
 * APIs to use the data structure
 */
void queue_initialize(queue_st *q);
int queue_eneque(queue_st *q, void *data, size_t len);
void *queue_deque(queue_st *q, int *status);
int queue_list(queue_st *q);
int queue_compact_queue(queue_st *q, size_t len);
queue_st* queue_alloc_queue(size_t len);
void *queue_alloc_memory(size_t len);
int queue_is_queue_empty(queue_st *q);

