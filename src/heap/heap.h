#include "../include/header.h"

#define HEAP_MAX_SIZE 50
#define HEAP_MIN_SIZE 0
#define HEAP_ROOT_LOC 0
#define HEAP_TYPE_MIN 0
#define HEAP_TYPE_MAX 1

/*
 * This is the structure for storing heap contents in memory.
 */
typedef struct heap
{

	void *heap_data[HEAP_MAX_SIZE];
	int heap_size;
	bool heap_type;

}heap_st;

typedef void (*heap_print_node_t) (void *);

/*
 * APIs used for manipulation of heap.
 */
heap_st *heap_allocate_heap(size_t len, bool heap_type);
int heap_deallocate_heap(heap_st *heap);

int heap_validate_input(heap_st *heap, bool type);
int heap_insert_min(heap_st *heap,
		    void *data,
		    size_t len,
		    common_data_compare_t compare);
int heap_insert_max(heap_st *heap,
		    void *data,
		    size_t len,
		    common_data_compare_t compare);
int heap_insert(heap_st *heap,
		void *data,
		size_t len,
		bool heap_type,
		common_data_compare_t compare);

int heap_get_parent(int i);
int heap_get_left_child(int i);
int heap_get_right_child(int i);

void heap_print_int_node(void *data);
int heap_print_heap(heap_st *heap, heap_print_node_t printer);

int heap_remove(heap_st *heap,
                void *data,
		size_t len,
                bool heap_type,
                common_data_compare_t compare);
int heap_remove_max(heap_st *heap,
		    void *data,
		    size_t len,
		    common_data_compare_t compare);
int heap_remove_min(heap_st *heap,
		    void *data,
		    size_t len,
		    common_data_compare_t compare);

int heap_max_heapify(heap_st *heap, int index, common_data_compare_t compare);
int heap_min_heapify(heap_st *heap, int index, common_data_compare_t compare);

