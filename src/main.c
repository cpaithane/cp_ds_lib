#include "include/header.h"

void cp_ds_lib_usage()
{

	printf("usage : cp_ds_lib <Name of data structure>\n");
	printf("usage : cp_ds_lib stack\n");

}

/*
 * This is workhorse function for execution of test cases.
 */
int cp_ds_lib_execute(char *str)
{

	int rc = EOK;

	if (strcmp(str, "stack") == 0)
	{
		rc = stack_tc_execute();
	}
	else if (strcmp(str, "queue") == 0)
	{
		rc = queue_tc_execute();
	}
	else if (strcmp(str, "bst") == 0)
	{
		rc = bst_tc_execute();
	}
	else if (strcmp(str, "sll") == 0)
	{
		rc = sll_tc_execute();
	}
	else if (strcmp(str, "dll") == 0)
	{
		rc = dll_tc_execute();
	}
	else if (strcmp(str, "scll") == 0)
	{
		rc = scll_tc_execute();
	}
	else if (strcmp(str, "avl_tree") == 0)
	{
		rc = avl_tc_execute();
	}
	else if (strcmp(str, "heap") == 0)
	{
		rc = heap_tc_execute();
	}
	else if (strcmp(str, "graph") == 0)
	{
		rc = graph_tc_execute();
	}
	else if (strcmp(str, "trie") == 0)
	{
		rc = trie_tc_execute();
	}
	else
	{
		CHECK_RC_ASSERT(1, 0);
	}

	return rc;

}

int main (int argc, char *argv[])
{

	int rc;

	if (argc <= 1)
	{
		
		cp_ds_lib_usage();
		return 1;

	}

	rc = cp_ds_lib_execute(argv[1]);
	return rc;

}

