# Preview
# About
This game was written for a C-course I attended some time ago. You can choose between 'X' or 'O' and 3 difficulties (easy, medium, unbeatable). A scoreboard is used to keep track of the points (draw = 0.5, win = 1). At the end of each round it is possible to change your current settings. For more details please look at states.pdf in the doc folder, which should give you an overview.
# Technical details
The UI is text-based using the ncurses-library. On "medium" and "unbeatable" difficulty the minimax algorithm is used to select a move. If "medium" difficulty is selected, the engine looks one move ahead (it tries to block moves which would let the opponent win or it makes a winning move by itself if it's possible). If "unbeatable" is selected, the engine goes full depth, which means it checks all possible outcomes of the game. The game-tree is build using a tree data-structure. A singly-linked list is used to represent the child nodes of each treenode.
# Installation
Assuming gcc is installed under ***/usr/bin/gcc*** please use the Makefile. Here are some options:

| Description | Command |
|---|---|
| compile and run | make all |
| compile | make compile |
| remove compiled file | make clean |

# Known issues
- Resizing of the terminal during execution doesn't resize the UI.
- Sometimes the engine doesn't win on the spot, if its possible. The reason for this behaviour is, that the engine knows, that the opponent can't win on the next move (the implemenation of the minimax considers winning on the current move equally good as winning on the next move).
- The same goes for loosing. If the engine knows the game is already lost (for example if the engine gets forked by the opponent) it just places its sign at some random postion.
