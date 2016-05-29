#include "sll.h"

/*
 * This function initilizes next pointer to NULL and copies input data to node->data.
 */
void sll_initialize_node(sll_st *node, void *data, size_t len)
{

	memcpy(node->sll_data, data, len);
	node->sll_next = NULL;

}

/*
 * This function allocates a node of singly linked list.
 */
sll_st *sll_alloc_node(size_t len, void *data)
{

	sll_st *node = NULL;
	node = (sll_st *)malloc(sizeof(sll_st));
	memset(node, 0, sizeof(sll_st));
	node->sll_data = malloc(len);
	memset(node->sll_data, 0, len);
	sll_initialize_node(node, data, len);
	
	return node;

}

/*
 * This function frees the individual node of sll
 */
void sll_dealloc_node(sll_st *node)
{

	free(node->sll_data);
	free(node);

}

/*
 * This function inserts new_node at head.
 */
sll_st *sll_insert_node_0(sll_st *head, sll_st *new_node)
{

	/*
	 * If linked list is empty then new_node will be head of linked list.
	 */
	if (head == NULL)
	{
		head = new_node;
	}
	/*
	 * Linked list is not empty. This is scenario where insertion is made at head of 
	 * linked list.
	 */
	else
	{

		new_node->sll_next = head;
		head = new_node;

	}
	
	return head;

}

/*
 * This function inserts node at nth position.
 */
sll_st *sll_insert_node_n(sll_st *head, sll_st *new_node, int position)
{

	int i = 0;
	sll_st *tmp = head;

	/*
	 * Traverse through the linked list to reach (position -1) node.
	 */
	while ((tmp->sll_next) && (i < position - 1))
	{

		i++;
		tmp = tmp->sll_next;

	}

	/*
	 * For e.g. linked list has 2 nodes and position specified is 5, then new node will
	 * be inserted at tail of linked list.
	 */
	new_node->sll_next = tmp->sll_next;
	tmp->sll_next = new_node;

	return head;

}

/*
 * This function inserts new node at specified position.
 */
sll_st *sll_insert_node_pos(sll_st *head, void *data, int position, size_t len)
{

	sll_st *new_node = NULL;

	new_node = sll_alloc_node(len, data);

	/*
	 * Insertion at the head of linked list
	 */
	if (position == 0)
	{
		head = sll_insert_node_0(head, new_node);
	}
	else 
	{
		head = sll_insert_node_n(head, new_node, position);
	}

	CHECK_RC_ASSERT((head == NULL), 0);
	return head;

}

/*
 * This function removes head node of the linked list
 */
sll_st *sll_remove_node_0(sll_st *head)
{

	sll_st *tmp;
	tmp = head;

	/*
	 * If head is only element, return NULL
	 */
	if (tmp->sll_next == NULL)
	{
		head = NULL;
	}
	/*
	 * If there are multiple nodes then adjust head
	 */
	else
	{
		head = tmp->sll_next;
	}

	sll_dealloc_node(tmp);
	
	return head;

}

/*
 * This function removes node at n position of the linked list.
 */
sll_st *sll_remove_node_n(sll_st *head, int position)
{

	int i;
	sll_st *tmp = NULL;
	sll_st *tmp2 = NULL;

	i = 0;
	tmp = head;

	/*
	 * Traverse through the linked list to reach to the position - 1 node.
	 */
	while ((tmp) && (tmp->sll_next) && 
               (i < position - 1))
	{

		i++;
		tmp2 = tmp;
		tmp = tmp->sll_next;

	}

	/*
	 * Non tail case.
	 */
	if (tmp->sll_next)
	{

		tmp2 = tmp->sll_next;
		tmp->sll_next = tmp2->sll_next;
		tmp2->sll_next = NULL;
		sll_dealloc_node(tmp2);

	}
	/*
	 * Tail case. free the tmp as is. 
	 * Should not come here. Position is wrong. 
	 */
	else
	{

		/*
		 * If only 1 node is present, return head as NULL.
		 */
		if (head->sll_next == NULL)
		{
			head = NULL;
		}

		/*
		 * Previous could be NULL
		 */
		if (tmp2)
		{
			tmp2->sll_next = NULL;
		}
		sll_dealloc_node(tmp);

	}

	return head;

}

/*
 * This function removes node at specified position.
 */
sll_st *sll_remove_node_pos(sll_st *head, int position)
{

	/*
	 * In case of empty linked list, return NULL.
	 */
	if (head == NULL)
	{
		return NULL;
	}

	if (position == 0)
	{

		head = sll_remove_node_0(head);
		return head;

	}
	else
	{

		head = sll_remove_node_n(head, position);
		return head;

	}

	return head;

}

/*
 * This function prints data inside node of linked list
 */
int sll_print_sll(sll_st *head)
{

	int rc = EOK;
	int *data = NULL;

	printf("\n");
	sll_st *temp = head;
	while (temp)
	{

		data = (int *)(temp->sll_data);
		printf("%d->", *data);
		temp = temp->sll_next;

	}

	printf("\n");
	return rc;

}

/*
 * This function deletes every node of linked list.
 */
int sll_remove_sll(sll_st *head)
{

	int i;
	int rc = EOK;

	i = 0;

	while (head)
	{

		i++;
		head = sll_remove_node_0(head);

	}

	CHECK_RC_ASSERT(head, NULL);
	return rc;

}

