struct list_node {
	struct tree_node *t_node;
	struct list_node *next;
};

struct list_node *initialize_list();

struct list_node *insert_after_node(struct tree_node *t_node,
				    struct list_node *l_node);

void delete_after_node(struct list_node *l_node);

struct list_node *get_nth_node(int n, struct list_node *head);

void delete_list(struct list_node *head);
