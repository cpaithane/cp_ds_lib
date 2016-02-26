#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

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

