#include "../include/header.h"

#define STACK_MAX_ITEMS 100
#define STACK_MIN_ITEMS -1

/*
 * Data structure for stack
 */
typedef struct stack{

	int st_top;
	void *st_node[STACK_MAX_ITEMS];

}stack_st;

/*
 * Supporting APIs for stack
 */
void stack_initialize(stack_st *st);
int stack_is_stack_full(stack_st *st);
int stack_is_stack_empty(stack_st *st);
int stack_push(stack_st *st, void *data, size_t len);
void *stack_pop(stack_st *st, int *status);
int stack_list(stack_st *st);
stack_st * stack_alloc_stack(size_t len);
int stack_dealloc_stack(stack_st *st);
void *stack_get_new_node(size_t len);
void *stack_get_peek(stack_st *st);

/*
 * Function declarations for problems solved.
 */
int stack_prob_1(char *input);
int stack_prob_2(char *input);
int stack_prob_3(char *input);
int stack_prob_4(char *input);

