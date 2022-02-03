#include "../include/board.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

const int m_board = 3;
const int max_turns = m_board * m_board;

static const int x_wins = 3 * 'X';
static const int o_wins = 3 * 'O';

/*
 * reset the board-state -> all cells empty
 */
void reset_board(char **board)
{
	for (int i = 0; i < m_board; i++)
		for (int j = 0; j < m_board; j++)
			board[i][j] = ' ';
}

/*
 * initialize a new board with all empty cells
 */
char **initialize_board()
{
	char **board = malloc(m_board * sizeof(char *));

	for (int i = 0; i < m_board; i++)
		board[i] = malloc(m_board * sizeof(char));

	reset_board(board);

	return board;
}

/*
 * delete the given board and free up its memory
 */
void delete_board(char **board)
{
	for (int i = 0; i < m_board; i++)
		free(board[i]);
	free(board);
}

/*
 * check if 'X' or 'O' wins by building the
 * horizontal, vertical and diagonal sum of all cell triples
 * return value:
 * CHAR_MAX = 127 <=> 'X' wins <=> 'O' looses
 * (CHAR_MIN + 1) = -127 <=> 'O' wins <=> 'X' looses
 * 0 <=> draw
 */
char check_winner(char **board)
{
	// horizontal
	for (int i = 0; i < m_board; i++) {
		int sum = 0;
		for (int j = 0; j < m_board; j++)
			sum += board[i][j];
		if (sum == x_wins)
			return CHAR_MAX;
		if (sum == o_wins)
			return (CHAR_MIN + 1);
	}

	// vertical
	for (int j = 0; j < m_board; j++) {
		int sum = 0;
		for (int i = 0; i < m_board; i++)
			sum += board[i][j];
		if (sum == x_wins)
			return CHAR_MAX;
		if (sum == o_wins)
			return (CHAR_MIN + 1);
	}

	// forward diagonal
	int sum = 0;
	for (int i = 0; i < m_board; i++)
		sum += board[i][i];
	if (sum == x_wins)
		return CHAR_MAX;
	if (sum == o_wins)
		return (CHAR_MIN + 1);
	// backward diagonal
	sum = 0;
	for (int j = 0; j < m_board; j++)
		sum += board[m_board - 1 - j][j];
	if (sum == x_wins)
		return CHAR_MAX;
	if (sum == o_wins)
		return (CHAR_MIN + 1);

	return 0;
}

/*
 * make a move at the given position as the given player('X' or 'O')
 * the position is given as a single coordinate which maps to to corresponding
 * 2-dimensional coordinates (0 -> 0/0, 1 -> 0/1 etc.).
 * return value:
 * 0 illegal move
 * 1 legal move - 'X' or 'O' was placed on given pos
 */
int make_move(char **board, char pos, char play_as)
{
	int i = pos / m_board;
	int j = pos % m_board;

	if ((i >= m_board || i < 0) || (j >= m_board || j < 0) ||
	    board[i][j] != ' ')
		return 0;

	board[i][j] = play_as;
	return 1;
}

/*
 * copy the boardstate of a given board and return the new board
 */
char **copy_boardstate(char **board)
{
	char **board_new = initialize_board();
	for (int i = 0; i < m_board; i++)
		for (int j = 0; j < m_board; j++)
			board_new[i][j] = board[i][j];

	return board_new;
}

/*
 * return all possible moves starting from the given boardstate
 * possible moves are returned as an one-dimensional char array containing positions
 * the function expects the current boardstate and the number of the the
 * current turn as arguments
 */
char *get_possible_moves(char **board, int n_turn)
{
	int rem_turns = max_turns - n_turn;
	char *moves_possible = malloc(rem_turns * sizeof(char));

	int k = 0;
	for (int i = 0; i < m_board; i++)
		for (int j = 0; j < m_board; j++)
			if (board[i][j] == ' ')
				moves_possible[k++] = i * m_board + j;

	return moves_possible;
}

/*
 * compare two given boardstates against each other
 * return value:
 * postion of first mismatch (mapped to 1-dimensional array)
 * -1 if boardstates are equal
 */
char get_board_diff(char **board, char **board_new)
{
	for (int i = 0; i < m_board; i++)
		for (int j = 0; j < m_board; j++)
			if (board[i][j] != board_new[i][j])
				return (i * m_board + j);
	return -1;
}
