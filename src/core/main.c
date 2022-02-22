#include <limits.h>
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include "../include/board.h"
#include "../include/engine.h"

#define WINHEIGHT 2

static const char game_title[] = "TIC-TAC-TOE";
static const char name_player[] = "PLAYER";
static const char name_computer[] = "COMPUTER";
static const char info_playas[] = "Play as(X, O): ";
static const char info_turn[] = "Your move(1-9): ";
static const char info_repeat[] = "Again(Y, N): ";
static const char info_option[] = "Change settings(C) or quit(Q): ";
static const char info_difficulty[] =
	"Select difficulty(1 = easy, 2 = medium, 3 = unbeatable): ";

static float p_points;
static float c_points;

void update_playfield(WINDOW *playfield, char **board)
{
	for (int i = 0; i < m_board; i++) {
		for (int j = 0; j < m_board; j++) {
			// map board i/j to corresponding cell in displayed window
			int x = 2 * j + 1;
			int y = 2 * i + 1;
			mvwprintw(playfield, y, x, "%c", board[i][j]);
		}
	}
	wrefresh(playfield);
}

void update_info(WINDOW *info, char const *msg)
{
	// clear old content
	int y_max, x_max, y, x;
	getmaxyx(info, y_max, x_max);
	for (int i = 0; i <= y_max; i++) {
		wmove(info, i, 0);
		wclrtoeol(info);
	}

	// set curs. pos and print
	y = 0;
	x = (x_max - strlen(msg)) / 2;
	mvwprintw(info, y, x, "%s", msg);

	wrefresh(info);
}

void update_scores(WINDOW *player_score, WINDOW *computer_score, char res,
		   char symbol_player)
{
	if (res == 0) {
		p_points += 0.5;
		c_points += 0.5;
		mvwprintw(player_score, 0, 0, "%s%.1f", name_player, p_points);
		wrefresh(player_score);
		mvwprintw(computer_score, 0, 0, "%s%.1f", name_computer,
			  c_points);
		wrefresh(computer_score);
	} else if ((res == CHAR_MAX && symbol_player == 'X') ||
		   (res == (CHAR_MIN + 1) && symbol_player == 'O')) {
		p_points++;
		mvwprintw(player_score, 0, 0, "%s%.1f", name_player, p_points);
		wrefresh(player_score);
	} else {
		c_points++;
		mvwprintw(computer_score, 0, 0, "%s%.1f", name_computer,
			  c_points);
		wrefresh(computer_score);
	}
}

int main()
{
	int y_max, x_max;
	int y, x;
	int height, width;

	int y_playfield, x_playfield;

	initscr();

	noecho();
	refresh();

	getmaxyx(stdscr, y_max, x_max);

	// playfield
	height = width = 2 * m_board + 1;
	y = (y_max - height) / 2;
	x = (x_max - width) / 2;
	WINDOW *playfield = newwin(height, width, y, x);
	for (int j = 0; j < width; j += 2)
		mvwvline(playfield, 0, j, ACS_VLINE, height);
	for (int i = 0; i < height; i += 2)
		mvwhline(playfield, i, 0, ACS_HLINE, width);
	box(playfield, 0, 0);
	wrefresh(playfield);
	getbegyx(playfield, y_playfield, x_playfield);

	// title
	height = WINHEIGHT;
	width = strlen(game_title);
	y = y_playfield / 2;
	x = (x_max - width) / 2;
	WINDOW *title = newwin(height, width, y, x);
	wprintw(title, "%s", game_title);
	wrefresh(title);

	// player score
	height = WINHEIGHT;
	width = strlen(name_player);
	y = (y_max - height) / 2;
	x = x_playfield / 2;
	WINDOW *p_score = newwin(height, width, y, x);
	wprintw(p_score, "%s%.1f", name_player, p_points);
	wrefresh(p_score);

	// computer score
	height = WINHEIGHT;
	width = strlen(name_computer);
	y = (y_max - height) / 2;
	x = (x_max + x_playfield) / 2;
	WINDOW *c_score = newwin(height, width, y, x);
	wprintw(c_score, "%s%.1f", name_computer, c_points);
	wrefresh(c_score);

	// infobox
	height = WINHEIGHT;
	width = strlen(info_difficulty) + 2;
	y = y_max - y_playfield / 2;
	x = (x_max - width) / 2;
	WINDOW *info = newwin(height, width, y, x);
	wprintw(info, "%s", info_difficulty);
	wrefresh(info);

	char main_loop = 'C';
	while (main_loop == 'C') {
		char difficulty;
		update_info(info, info_difficulty);
		do
			difficulty = wgetch(info);
		while (difficulty < '1' || difficulty > '3');

		char symbol_player;
		char symbol_computer;
		update_info(info, info_playas);
		do
			symbol_player = wgetch(info);
		while (symbol_player != 'X' && symbol_player != 'O');
		if (symbol_player == 'X')
			symbol_computer = 'O';
		else
			symbol_computer = 'X';

		char **board = initialize_board();
		char game_loop = 'Y';
		while (game_loop == 'Y') {
			char res;

			for (int i = 0;
			     (res = check_winner(board)) == 0 && i < max_turns;
			     i++) {
				char move;
				if (symbol_player == 'X') {
					if (i % 2 == 0) {
						update_info(info, info_turn);
						do
							move = wgetch(info) -
							       '1';
						while (make_move(
							       board, move,
							       symbol_player) ==
						       0);
					} else {
						char pos = select_move(
							copy_boardstate(board),
							difficulty, i);
						make_move(board, pos,
							  symbol_computer);
					}
				} else {
					if (i % 2 == 0) {
						char pos = select_move(
							copy_boardstate(board),
							difficulty, i);
						make_move(board, pos,
							  symbol_computer);
					} else {
						update_info(info, info_turn);
						do
							move = wgetch(info) -
							       '1';
						while (make_move(
							       board, move,
							       symbol_player) ==
						       0);
					}
				}
				update_playfield(playfield, board);
			}

			update_scores(p_score, c_score, res, symbol_player);

			update_info(info, info_repeat);
			do
				game_loop = wgetch(info);
			while (game_loop != 'Y' && game_loop != 'N');

			reset_board(board);
			update_playfield(playfield, board);
		}
		delete_board(board);

		// change settings or quit
		update_info(info, info_option);
		do
			main_loop = wgetch(info);
		while (main_loop != 'Q' && main_loop != 'C');
	}

	delwin(playfield);
	delwin(title);
	delwin(p_score);
	delwin(c_score);
	delwin(info);
	delwin(stdscr);

	endwin();

	return 0;
}
