#include "scll.h"

/*
 * This function allocates a singly circular node
 */
scll_st *scll_alloc_node(size_t len, void *data)
{

	scll_st *node = NULL;

	node = (scll_st *)malloc(sizeof(scll_st));
	memset(node, 0, sizeof(scll_st));

	node->scll_data = malloc(len);
	memset(node->scll_data, 0, len);

	scll_initialize(node, data, len);

	return node;

}

/*
 * This function frees scll node.
 */
void scll_dealloc_node(scll_st *node)
{

	free(node->scll_data);
	free(node);

}

/*
 * This function copies input data into node. It sets next pointer to the self.
 */
void scll_initialize(scll_st *node, void *data, size_t len)
{

	memcpy(node->scll_data, data, len);
	node->scll_next = node;

}

/*
 * This function traverses scll till position'th node.
 * If position = -1 is specified, then traverse till tail of linked list.
 */
int scll_traverse_list(scll_st *head, 
			scll_st **prev, 
			scll_st **node, 
			scll_st **next, 
			int position)
{

	int rc = EOK;
	int i = 0;
	int traverse_till_tail = (position == -1);
	scll_st *tmp = head;
	*prev = NULL;
	*next = head->scll_next;

	while ((tmp) && (tmp->scll_next != head))
	{

		i++;
		*prev = tmp;
		*next = tmp->scll_next;
		tmp = tmp->scll_next;

		if (traverse_till_tail)
			continue;

		if (i >= position)
		{
			break;
		}
	}

	*node = tmp;
	return rc;

}

/*
 * This function inserts new_node at head of scll
 */
scll_st *scll_insert_node_0(scll_st *head, scll_st *new_node)
{

	scll_st *tail = NULL;
	scll_st *prev = NULL;
	scll_st *next = NULL;
	scll_st *tmp = head;

	/*
	 * In case of empty scll, return new_node as is.
	 */
	if (head == NULL)
	{
		head = new_node;
	}
	/*
	 * In case of non-empty scll, modify pointer as follow.
	 */
	else
	{

		/*
		 * Get tail node of scll to adjust tail's next pointer.
		 */
		scll_traverse_list(head, &prev, &tail, &next, MAX_INT);

		tail->scll_next = new_node;
		new_node->scll_next = head;
		head = new_node;

	}

	return head;

}

/*
 * This function inserts new_node at position.
 */
scll_st *scll_insert_node_n(scll_st *head, scll_st *new_node, int position)
{

	scll_st *tmp = NULL;
	scll_st *tmp2 = NULL;
	scll_st *prev = NULL;
	scll_st *next = NULL;

	/*
	 * Traverse till one node prior to position (tmp).
	 * tmp2 is next node of tmp
	 */
	scll_traverse_list(head, &prev, &tmp, &next, (position-1));
	tmp2 = tmp->scll_next;

	/*
	 * If insertion is targeted at non-tail node, adjust next pointer to next->next 
	 * pointer.
	 */
	if (tmp->scll_next != head)
	{

		tmp->scll_next = new_node;
		new_node->scll_next = tmp2;

	}
	/*
	 * If insertion is targeted at tail node, adjust next pointer to head.
	 */
	else
	{

		tmp->scll_next = new_node;
		new_node->scll_next = head;

	}

	return head;

}

/*
 * This function is work-horse for insertion.
 */
scll_st *scll_insert_node_pos(scll_st *head, void *data, int position, size_t len)
{

	scll_st *new_node = NULL;

	new_node = scll_alloc_node(len, data);

	if (position == 0)
	{
		head = scll_insert_node_0(head, new_node);
	}
	else
	{
		head = scll_insert_node_n(head, new_node, position);
	}

	return head;

}

/*
 * This function removes node at head
 */
scll_st *scll_remove_node_0(scll_st *head)
{

	scll_st *tmp = head;
	scll_st *tail = NULL;
	scll_st *prev = NULL;
	scll_st *next = NULL;

	/*
	 * If a single node in scll is present, dealloc and return NULL.
	 */
	if ((tmp->scll_next) == head)
	{
		head = NULL;
	}
	/*
	 * If there are multiple nodes, then readjust pointers.
	 */
	else
	{

		scll_traverse_list(head, &prev, &tail, &next, MAX_INT);
		head = tmp->scll_next;
		tail->scll_next = head;

	}

	scll_dealloc_node(tmp);
	return head;

}

/*
 * This function removes node at specified position.
 */
scll_st *scll_remove_node_n(scll_st *head, int position)
{

	scll_st *tmp = head;
	scll_st *tmp2 = head;
	scll_st *prev = NULL;
	scll_st *next = NULL;
	int i = 0;
	
	/*
	 * Traverse to a (position - 1)th node.
	 */
	scll_traverse_list(head, &prev, &tmp, &next, (position - 1));
	tmp2 = tmp->scll_next;

	if (tmp->scll_next != head)
	{
		tmp->scll_next = tmp2->scll_next;
	}
	else
	{

		/*
		 * If prev is not NULL then there are multiple nodes. Else, head is single node 
		 * present in scll.
		 */
		if (prev)
		{

			prev->scll_next = head;
			tmp2 = tmp;

		}
		else
		{

			tmp2 = head;
			head = NULL;
		
		}

	}

	scll_dealloc_node(tmp2);
	return head;

}

/*
 * This function is a workhorse function to remove node at any position. 
 */
scll_st *scll_remove_node_pos(scll_st *head, int position)
{

	if (position == 0)
	{
		head = scll_remove_node_0(head);
	}
	else
	{
		head = scll_remove_node_n(head, position);
	}

	return head;

}

/*
 * This function removes all nodes at position 0 one bye one.
 */
int scll_remove_scll(scll_st *head)
{

	int rc = EOK;

	while ((head) && (head->scll_next != head))
	{
		head = scll_remove_node_0(head);
	}

	/*
	 * Head here will remain allocated. So, deallocate it.
	 */
	if (head)
	{

		scll_dealloc_node(head);
		head = NULL;

	}

	CHECK_RC_ASSERT(head, NULL);	
	return rc;

}

/*
 * This function prints scll. Stopping point is when pointer is pointing to head of 
 * scll.
 */
int scll_print_list(scll_st *head)
{

	scll_st *tmp1 = head;

	printf("\n");

	while((tmp1) && (tmp1->scll_next != head))
	{

		SCLL_PRINT_NODE(tmp1);
		tmp1 = tmp1->scll_next;

	}

	/*
	 * Print last element in scll
	 */
	if ((tmp1) && (tmp1->scll_next == head))
	{
		SCLL_PRINT_NODE(tmp1);
	}

	printf("\n");
	return EOK;

}

