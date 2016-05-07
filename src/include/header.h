#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <limits.h>
#include <stdbool.h>

#define EOK 0
#define MAX_INT ((unsigned int)(-1))
#define FIRST_LESS -1
#define FIRST_GREATER 1
#define IDENTICAL 0

#define MAX(a, b) ((a > b) ? (a) : (b))

#define CHECK_RC(actual_rc, expected_rc)\
do {\
	\
	if (actual_rc != expected_rc)	\
		return actual_rc;	\
	\
}while(0)

#define CHECK_RC_ASSERT(actual_rc, expected_rc)\
do {\
	\
	if (actual_rc != expected_rc)	\
		assert(0);	\
	\
}while(0)

#define CHECK_BOOL_RET_RC(actual_bool, expected_bool, rc)\
do {\
	\
	if (actual_bool != expected_bool)	\
		return rc;	\
	\
}while(0)

/*
 * Common APIs to be used.
 */
typedef int (*common_data_compare_t)(const void *, const void *);
int common_int_data_compare(const void *data1, const void *data2);

/*
 * Following functions are entry points for test cases.
 */
int stack_tc_execute();
int queue_tc_execute();
int bst_tc_execute();
int sll_tc_execute();
int dll_tc_execute();
int scll_tc_execute();
int heap_tc_execute();
