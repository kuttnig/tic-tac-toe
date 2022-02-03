#include "list.h"
#include "board.h"

struct tree_node {
	char **board;
	char eval;
	struct list_node *children;
};

struct tree_node *initialize_tree(char **);

void delete_tree(struct tree_node *);

// insert a new node as child of the given node
struct tree_node *insert_child_node(char **, struct tree_node *);
