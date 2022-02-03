extern const int m_board;
extern const int max_turns;

void reset_board(char **);

char **initialize_board();

void delete_board(char **);

char check_winner(char **);

int make_move(char **, char, char);

char **copy_boardstate(char **);

char *get_possible_moves(char **, int);

char get_board_diff(char **, char **);
