#include "../include/tree.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * initialize new linked-list
 * head points to t and t points to itself 
 */
struct list_node *initialize_list()
{
	struct list_node *head = malloc(sizeof(*head));
	struct list_node *t = malloc(sizeof(*t));

	head->next = t;
	t->next = t;

	return head;
}

/*
 * insert a new node after the given node
 */
struct list_node *insert_after_node(struct tree_node *t_node,
				    struct list_node *l_node)
{
	struct list_node *n = malloc(sizeof(*n));
	n->t_node = t_node;

	n->next = l_node->next;
	l_node->next = n;

	return n;
}

/*
 * delete the node next to the given node
 */
void delete_after_node(struct list_node *l_node)
{
	struct list_node *tmp = l_node->next;
	l_node->next = l_node->next->next;
	free(tmp);
}

/*
 * delete linked-list and free up memory
 * expects head of the list as argument
 */
void delete_list(struct list_node *head)
{
	struct list_node *node = head;

	while (node->next != node) {
		head = node->next;
		free(node);
		node = head;
	}
	free(node);
}
