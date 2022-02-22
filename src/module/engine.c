#include "../include/engine.h"
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define EASY '1'
#define MEDIUM '2'

/*
 * build the game-tree for the minimax function
 * the function expects a tree-node with the current board-state
 * , a given depth (= height of tree) and the number of the current turn as arguments
 * return value:
 * pointer to the root of the game-tree
 */
struct tree_node *build_gametree(struct tree_node *t_node, int depth,
				 int n_turn)
{
	if (depth > 0) {
		char play_as;
		if (n_turn % 2 == 0)
			play_as = 'X';
		else
			play_as = 'O';

		char *moves_possible =
			get_possible_moves(t_node->board, n_turn);

		int rem_turns = max_turns - n_turn;
		for (int i = 0; i < rem_turns; i++) {
			char **board_new = copy_boardstate(t_node->board);
			make_move(board_new, moves_possible[i], play_as);
			struct tree_node *parent =
				insert_child_node(board_new, t_node);

			if (check_winner(board_new) == 0)
				build_gametree(parent, (depth - 1),
					       (n_turn + 1));
		}
		free(moves_possible);
	}

	return t_node;
}

char min(char a, char b)
{
	return a < b ? a : b;
}

char max(char a, char b)
{
	return a > b ? a : b;
}

/*
 * minimax function expects the game-tree a given depth and a boolean value if
 * current player is maximizing or not
 * return value:
 * evaluation value (this value is equal to at least one child node of the root)
 */
char minimax(struct tree_node *t_node, int depth, char is_maximizing)
{
	if (depth == 0 ||
	    t_node->children->next == t_node->children->next->next) {
		t_node->eval = check_winner(t_node->board);
		return t_node->eval;
	}

	struct list_node *child = t_node->children->next;
	if (is_maximizing) {
		t_node->eval = CHAR_MIN + 1;
		while (child != child->next) {
			t_node->eval =
				max(t_node->eval,
				    minimax(child->t_node, (depth - 1), 0));
			child = child->next;
		}
		return t_node->eval;
	} else {
		t_node->eval = CHAR_MAX;
		while (child != child->next) {
			t_node->eval =
				min(t_node->eval,
				    minimax(child->t_node, (depth - 1), 1));
			child = child->next;
		}
		return t_node->eval;
	}
}

/*
 *  function for selecting a move by a given difficulty
 */
char select_move(char **board, char difficulty, int n_turn)
{
	if (difficulty == EASY)
		return select_move_easy(board, n_turn);
	if (difficulty == MEDIUM)
		return select_move_medium(board, n_turn);
	return select_move_hard(board, n_turn);
}

/*
 * select a legal move at random
 */
char select_move_easy(char **board, int n_turn)
{
	int rem_turns = max_turns - n_turn;
	char *moves_possible = get_possible_moves(board, n_turn);

	srand(time(NULL));
	char move = moves_possible[rand() % rem_turns];
	free(moves_possible);

	return move;
}

/*
 * select a legal move by using minimax with depth 2
 * if there are more possibilies which are considered equally good 
 * the function picks a good move at random
 */
char select_move_medium(char **board, int n_turn)
{
	int rem_turns = max_turns - n_turn;
	char is_maximizing;
	if (n_turn % 2 == 0)
		is_maximizing = 1;
	else
		is_maximizing = 0;
	int depth;
	if (rem_turns > 2)
		depth = 2;
	else
		depth = rem_turns;

	struct tree_node *gametree =
		build_gametree(initialize_tree(board), depth, n_turn);
	char val = minimax(gametree, depth, is_maximizing);

	struct list_node *head = gametree->children;
	struct list_node *child = head->next;
	int n = 0;
	while (child != child->next) {
		if (child->t_node->eval == val)
			n++;
		child = child->next;
	}
	char *possible_moves = malloc(n * sizeof(char));
	child = head->next;
	int i = 0;
	while (child != child->next) {
		if (child->t_node->eval == val)
			possible_moves[i++] =
				get_board_diff(board, child->t_node->board);
		child = child->next;
	}
	srand(time(NULL));
	char move = possible_moves[rand() % n];

	free(possible_moves);
	free(gametree);

	return move;
}

/*
 * select a move by using minimax with max-depth
 * if there are more possibilies which are considered equally good 
 * the function picks a good move at random
 */
char select_move_hard(char **board, int n_turn)
{
	int rem_turns = max_turns - n_turn;
	char is_maximizing;
	if (n_turn % 2 == 0)
		is_maximizing = 1;
	else
		is_maximizing = 0;

	struct tree_node *gametree =
		build_gametree(initialize_tree(board), rem_turns, n_turn);
	char val = minimax(gametree, rem_turns, is_maximizing);

	struct list_node *head = gametree->children;
	struct list_node *child = head->next;
	int n = 0;
	while (child != child->next) {
		if (child->t_node->eval == val)
			n++;
		child = child->next;
	}
	char *possible_moves = malloc(n * sizeof(char));
	child = head->next;
	int i = 0;
	while (child != child->next) {
		if (child->t_node->eval == val)
			possible_moves[i++] =
				get_board_diff(board, child->t_node->board);
		child = child->next;
	}
	srand(time(NULL));
	char move = possible_moves[rand() % n];

	free(possible_moves);
	delete_tree(gametree);

	return move;
}
