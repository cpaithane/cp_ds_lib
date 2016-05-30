#include "dll.h"

/*
 * This function initializes doubly linked list node.
 */
void dll_initialize(dll_st *node, void *data, size_t len)
{

	memcpy(node->dll_data, data, len);
	node->dll_prev = NULL;
	node->dll_next = NULL;

}

/*
 * This function allocates new node and data inside a node.
 */
dll_st *dll_alloc_node(void *data, size_t len)
{

	dll_st *node = NULL;

	node = (dll_st *)malloc(sizeof(dll_st));
	memset(node, 0, sizeof(dll_st));

	node->dll_data = malloc(len);
	memset(node->dll_data, 0, len);
	dll_initialize(node, data, len);

	return node;

}

/*
 * This function deallocates node and data inside it.
 */
void dll_dealloc_node(dll_st *node)
{

	free(node->dll_data);
	free(node);

}

/*
 * This function inserts node at specified position.
 */
dll_st *dll_insert_node_n(dll_st *head, dll_st *new_node, int position)
{

	dll_st *tmp = head;
	dll_st *prev = NULL;
	dll_st *next = NULL;
	dll_st *node = NULL;

	/*
	 * Traverse till (position - 1)th node.
	 */
	dll_traverse_list(head, &prev, &node, &next, (position - 1));

	/*
	 * If insertion is targetted at non-tail node
	 */
	if (node->dll_next != NULL)
	{

		node->dll_next = new_node;
		new_node->dll_prev = node;
		new_node->dll_next = next;
		next->dll_prev = new_node;

	}
	else
	{

		node->dll_next = new_node;
		new_node->dll_prev = node;

	}

	return head;

}

/*
 * This function inserts node at head position.
 */
dll_st *dll_insert_node_0(dll_st *head, dll_st *new_node)
{

	dll_st *next = NULL;

	/*
	 * If head is NULL, it is first insertion.
	 */
	if (head == NULL)
	{
		head = new_node;
	}
	/*
	 * Head is not NULL, adjust  
	 */
	else
	{

		new_node->dll_next = head;
		head->dll_prev = new_node;
		head = new_node;

	}

	return head;

}

/*
 * This function is workhorse for insertion at any position.
 */
dll_st *dll_insert_node_pos(dll_st *head, void *data, size_t len, int position)
{

	dll_st *new_node = NULL;
	new_node = dll_alloc_node(data, len);

	if (position == 0)
	{
		head = dll_insert_node_0(head, new_node);
	}
	else
	{
		head = dll_insert_node_n(head, new_node, position);
	}

	return head;

}

/*
 * This function removes node from head.
 */
dll_st *dll_remove_node_0(dll_st *head)
{

	dll_st *tmp = head;
	dll_st *tmp2 = head;

	/*
	 * If head is NULL, return.
	 */
	if (tmp == NULL)
	{
		return NULL;
	}

	/*
	 * If single node is present, delete it.
	 */
	if (tmp->dll_next == NULL)
	{
		head = NULL;
	}
	/*
	 * If multiple nodes are present, adjust pointers
	 */
	else
	{

		tmp2 = tmp->dll_next;
		tmp2->dll_prev = NULL;
		tmp->dll_next = NULL;
		head = tmp2;

	}

	dll_dealloc_node(tmp);
	return head;

}

/*
 * This function removes node at specified position.
 */
dll_st *dll_remove_node_n(dll_st *head, int position)
{

	dll_st *tmp = head;
	dll_st *tmp2 = NULL;
	dll_st *prev = NULL;
	dll_st *next = NULL;
	dll_st *node = NULL;

	/*
	 * Traverse till (position - 1)th node.
	 */
	dll_traverse_list(head, &prev, &node, &next, (position - 1));
	tmp2 = node->dll_next;

	if (node->dll_next != NULL)
	{

		node->dll_next = tmp2->dll_next;

		if (tmp2->dll_next)
		{
			tmp2->dll_next->dll_prev = node;
		}

	}
	else
	{

		node->dll_prev = NULL;
		if (prev)
		{
			prev->dll_next = NULL;
		}

		tmp2 = node;

	}

	dll_dealloc_node(tmp2);

	return head;

}

/*
 * This function is workhorse to remove dll node.
 */
dll_st *dll_remove_node_pos(dll_st *head, int position)
{

	if (position == 0)
	{
		head = dll_remove_node_0(head);
	}
	else
	{
		head = dll_remove_node_n(head, position);
	}

	return head;

}

/*
 * This function traverses list till end of the list or specified position.
 */
int dll_traverse_list(dll_st *head, 
		      dll_st **prev, 
		      dll_st **node, 
		      dll_st **next, 
		      int position)
{

	int rc = EOK;
	int i = 0;
	int traverse_till_tail = (position == -1);
	dll_st *tmp = head;
	*prev = NULL;
	*next = head->dll_next;

	while ((tmp) && (tmp->dll_next != NULL))
	{

		i++;
		tmp = tmp->dll_next;

		if (traverse_till_tail)
			continue;

		if (i >= position)
		{
			break;
		}
	}

	*node = tmp;
	*prev = tmp->dll_prev;
	*next = tmp->dll_next;
	return rc;

}

/*
 * This function empties whole dll.
 */
int dll_remove_dll(dll_st *head)
{

	int i = 0;
	int rc = EOK;

	while (head)
	{
	
		head = dll_remove_node_pos(head, 0);
		i++;

	}

	CHECK_RC_ASSERT(head, NULL);
	return rc;

}

/*
 * This function iterate through dll and print data in it.
 */
int dll_print_list(dll_st *head)
{

	int rc = EOK;
	dll_st *tmp = head;

	printf("\nPrinting in straight \n");
	while (tmp != NULL)
	{

		DLL_PRINT_NODE(tmp);
		tmp = tmp->dll_next;

	}

	printf("\n");
	return rc;

}

/*
 * This function prints list in reverse fashion.
 */
int dll_print_list_rev(dll_st *head)
{

	int rc = EOK;
	dll_st *tmp = NULL;
	dll_st *tail = NULL;
	dll_st *prev = NULL;
	dll_st *next = NULL;

	printf("\nPrinting in reverse\n");

	rc = dll_traverse_list(head, &prev, &tail, &next, -1);
	tmp = tail;
	while (tmp != NULL)
	{

		DLL_PRINT_NODE(tmp);
		tmp = tmp->dll_prev;

	}

	printf("\n");
	return rc;

}
