#include "tree.h"

struct tree_node *build_gametree(struct tree_node *, int, int);

char min(char, char);

char max(char, char);

char minimax(struct tree_node *, int, char);

char select_move(char **, char, int);

char select_move_easy(char **, int);

char select_move_medium(char **, int);

char select_move_hard(char **, int);
