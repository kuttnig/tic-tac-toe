#include "../include/tree.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * initialize a new tree
 * each node holds a board state(board), the evaluation value(default=0)
 * and a pointer to the head of a linked list, which represents the childs of
 * a tree-node
 */
struct tree_node *initialize_tree(char **board)
{
	struct tree_node *root = malloc(sizeof(*root));
	root->board = board;
	root->eval = 0;
	root->children = initialize_list();

	return root;
}

/*
 * delete a given tree and free up the memory of its contents
 * expects the root of the tree as argument
 */
void delete_tree(struct tree_node *t_node)
{
	struct list_node *head = t_node->children;

	struct list_node *child = head->next;
	while (child != child->next) {
		delete_tree(child->t_node);
		child = child->next;
	}

	delete_list(head);
	delete_board(t_node->board);
	free(t_node);
}

/*
 * insert a child node with the given boardstate as the child of the given node
 */
struct tree_node *insert_child_node(char **board, struct tree_node *parent)
{
	struct tree_node *child = malloc(sizeof(*child));
	child->board = board;
	child->eval = 0;
	child->children = initialize_list();

	// insert child into linked-list of parent after head
	insert_after_node(child, parent->children);

	return child;
}
