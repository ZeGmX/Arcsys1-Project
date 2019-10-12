
/* Template of the 7 wonders of the world of the 7 colors assigment. */

#include <stdio.h>     // printf //
#include <stdlib.h>     //random
#include <time.h>     //to initialize random



//TODO : include BOARD_SIZE and NB_COLORS into the struct

/* We want a 30x30 board game by default */
#define BOARD_SIZE 30
#define NB_COLORS 7 // 7 colors (+ 2 players)
//#define MAX(a, b) ((a)<(b)? (b) : (a)) //max between two numbers



/**************************************************************/
//                     Data structure                         //
/**************************************************************/


enum colors {
  A, B, C, D, E, F, G, PLAYER1, PLAYER2
};

typedef struct board_game board_game;

struct board_game {
  enum colors *game; //the board game
  enum colors turn; //the player who has to play next
  int nb_player1;   //number of positions that player 1 has conquered
  int nb_player2;   //same for player 2
};
/** Represent the actual current board game
 *
 * NOTE: global variables are usually discouraged (plus encapsulation in
 *     an appropriate data structure would also be preferred), but don't worry.
 *     For this first assignment, no dinosaure will get you if you do that.
 */





 /**************************************************************/
 //                     Useful functions                       //
 /**************************************************************/


//retrieves the color of a given board cell
enum colors get_cell(int x, int y, board_game *board) {
    return board->game[y * BOARD_SIZE + x];
}

//changes the color of a given board cell
void set_cell(int x, int y, enum colors color, board_game *board) {
    board->game[y * BOARD_SIZE + x] = color;
}

//copy the second tab into the first one
void copy_game(enum colors *copy, board_game *board) {
  for (int x = 0 ; x < BOARD_SIZE ; x++) {
    for (int y = 0 ; y < BOARD_SIZE ; y++) {
      copy[y * BOARD_SIZE + x] = get_cell(x, y, board);
    }
  }
}

//allows us to transform a color into its char representation
char colors_to_char(enum colors color) {
  switch(color) {       //TODO (maybe) : use a smarter way using pointer arithmetic and ascii representation
      case A:
        return 'A';
      case B:
        return 'B';
      case C:
        return 'C';
      case D:
        return 'D';
      case E:
        return 'E';
      case F:
        return 'F';
      case G:
        return 'G';
      case PLAYER1:
        return '^';
      case PLAYER2:
        return 'v';
      default:
        return 0;
  }
}

//turns a color into its integer index
int colors_to_int(enum colors color) {
  switch (color) {
    case A:
      return 0;
    case B:
      return 1;
    case C:
      return 2;
    case D:
      return 3;
    case E:
      return 4;
    case F:
      return 5;
    case G:
      return 6;
    case PLAYER1:
      return 7;
    case PLAYER2:
      return 8;
    default:
    return -1;
  }
}

//checks if x, y is a correct cell
int cell_in_bound(int x, int y) {
  if (x < BOARD_SIZE && x >= 0 && y < BOARD_SIZE && y >= 0) {
    return 1;
  } else {
    return 0;
  }
}

//set the cell conquered to the correct color and increase the number of cell conquered
//used to update the board
void player_conquered_one_cell(int x, int y, board_game *board) {
  if (board->turn == PLAYER1) {
    board->nb_player1++;
  }
  else {
    board->nb_player2++;
  }
  set_cell(x, y, board->turn, board);
}

//prints the current state of the board on screen
void print_board(board_game *board) {
    int i, j;
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
          enum colors color =  get_cell(j, i, board);
          printf("%c", colors_to_char(color));
        }
        printf("\n");
    }
}

//initialize the board by picking a random color for each position
board_game init_board() {
  static enum colors tab[BOARD_SIZE * BOARD_SIZE] = {(enum colors) A}; //DO NOT REMOVE THE STATIC. OTHERWHISE YOU COULD EXPECT HOURS OF HEAD SCRATCHING
  for (int y = 0 ; y < BOARD_SIZE ; y++) {
    for (int x = 0 ; x < BOARD_SIZE ; x++) {
      int color_index = rand() % NB_COLORS;
      enum colors color = A;
      color += color_index;
      tab[y * BOARD_SIZE + x] = color;
    }
  }
  enum colors p1 = PLAYER1;
  enum colors p2 = PLAYER2;
  tab[BOARD_SIZE - 1] = p1;//first player
  tab[(BOARD_SIZE - 1) * BOARD_SIZE] = p2;//second player
  board_game board = {tab, p1, 1, 1};
  return board;
}

//update one position next to (x, y) in the direction (dx, dy) where dx + dy = 1, dxdy = 0
//used in update_around_cell
void update_one_dir(int x, int y, int dx, int dy, enum colors color_played, int* has_done_something, board_game *board) {
  if (cell_in_bound(x + dx, y + dy)) { //checking if the position isn't out of the board
    if (get_cell(x + dx, y + dy, board) == color_played) {
      player_conquered_one_cell(x + dx, y + dy, board);
      *has_done_something = 1;
    }
  }
}

//update the 4 adjacent positions
//used in  update_board
int update_around_cell (int x, int y, enum colors color_played, board_game *board) { //returns 1 if something has changed, 0 otherwhise
  int has_done_something = 0;
  if (get_cell(x, y, board) == board->turn) {
    update_one_dir(x, y, -1, 0, color_played, &has_done_something, board);
    update_one_dir(x, y, 1, 0, color_played, &has_done_something, board);
    update_one_dir(x, y, 0, -1, color_played, &has_done_something, board);
    update_one_dir(x, y, 0, 1, color_played, &has_done_something, board);
  }
  return has_done_something;
}

//modify the board according to the player's move
void update_board(enum colors color_played, board_game *board) { //player = "^" ou "v" et color_played = A...G
  int has_done_something = 1;
  while (has_done_something) {
    has_done_something = 0;
    for (int y = 0 ; y < BOARD_SIZE ; y++) {
      for (int x = 0 ; x < BOARD_SIZE ; x++) {
        has_done_something =  update_around_cell(x, y, color_played, board) || has_done_something;
      }
    }
  }
  if (board->turn == PLAYER1) {
    board->turn = PLAYER2;
  }
  else {
    board->turn = PLAYER1;
  }
}

//updates the board. Finds new cells to add with a depth-first-search starting from the bottom-left or top-right corner
void depth_first_search_and_color (enum colors color_played, int *visited_cells, int x, int y, board_game *board) {
  void dfs_conqueror(enum colors color_played, int *visited_cells, int x, int y, int dx, int dy, board_game *board) {
    //auxiliary function for the dfs search
    int new_x = x + dx;
    int new_y = y + dy;
    if (cell_in_bound(new_x, new_y)) {
      if (get_cell(new_x, new_y, board) == color_played || get_cell(new_x, new_y, board) == board->turn) {
        if (!(visited_cells[new_y * BOARD_SIZE + new_x])) {

          if (get_cell(new_x, new_y, board) == color_played) {
            player_conquered_one_cell(new_x, new_y, board);
          }
          depth_first_search_and_color(color_played, visited_cells, new_x, new_y, board);
        }
      }
    }
  };
  visited_cells[y * BOARD_SIZE + x] = 1;
  dfs_conqueror(color_played, visited_cells, x, y, -1, 0, board);
  dfs_conqueror(color_played, visited_cells, x, y, 1, 0, board);
  dfs_conqueror(color_played, visited_cells, x, y, 0, -1, board);
  dfs_conqueror(color_played, visited_cells, x, y, 0, 1, board);
}

//same as update_board but smarter
void update_board_dfs(enum colors color_played, board_game *board) {
  int x, y; //they will contain the initial position for the search
  if (board->turn == PLAYER1) {
    x = BOARD_SIZE - 1;
    y = 0;
  }
  else {
    x = 0;
    y = BOARD_SIZE - 1;
  }

  int visited_cells[BOARD_SIZE * BOARD_SIZE] = { 0 };
  depth_first_search_and_color(color_played, visited_cells, x, y, board);

  if (board->turn == PLAYER1) {
    board->turn = PLAYER2;
  }
  else {
    board->turn = PLAYER1;
  }
}

//checks if the color at x+dx y+dy is the same as the one at x y
//used in perimeter
int different_neighbour(int x, int y, int dx, int dy, board_game *board) {
  if (cell_in_bound(x + dx, y + dy)) {
    if (get_cell(x + dx, y + dy, board) !=  get_cell(x, y, board)) {
      return 1;
    }
  }
  return 0;
}

//calculates the perimeter of the shape composed of the positions conquered by the player
int perimeter(board_game *board, enum colors player_color) {
  int res = 0;
  for (int y = 0 ; y < BOARD_SIZE ; y++) {
    for (int x = 0 ; x < BOARD_SIZE ; x++) {
      if (get_cell(x, y, board) == player_color) {
        res += different_neighbour(x, y, 1, 0, board);
        res += different_neighbour(x, y, -1, 0, board);
        res += different_neighbour(x, y, 0, 1, board);
        res += different_neighbour(x, y, 0, -1, board);
      }
    }
  }
  return res;
}

//each player playes when it's his turn, until one has conquered at least half of the board
void full_game(enum colors (*p1)(), enum colors (*p2)(), board_game *board, int wanna_print) { //game between two payers p1 and p2 with the game board
  enum colors color;
  int k = 0;
  while(board->nb_player1 < BOARD_SIZE * BOARD_SIZE / 2 && board->nb_player2 < BOARD_SIZE * BOARD_SIZE / 2) {
    if (wanna_print) {
      printf("\n\n");
    }
    if (k == 0) {
      color = p1(board);
    }
    else {
      color = p2(board);
    }
    if (wanna_print) {
      printf("player %d played color %c\n", k + 1, colors_to_char(color));
    }

    update_board_dfs(color, board);
    //update_board(color, board);
    if (wanna_print) {
      print_board(board);
      printf("Player 1 has conquered %d percent(s)\n", board->nb_player1 * 100 / (BOARD_SIZE * BOARD_SIZE));
      printf("Player 2 has conquered %d percent(s)\n", board->nb_player2 * 100 / (BOARD_SIZE * BOARD_SIZE));
    }
    k = (k + 1) % 2;
  }
  if (board->nb_player1 > board->nb_player2) {
    printf("Player 1 won the game with %d percents of the board\n", 100 * board->nb_player1 / (BOARD_SIZE * BOARD_SIZE));
  }
  else {
    printf("Player 2 won the game with %d percents of the board\n", 100 * board->nb_player2 / (BOARD_SIZE * BOARD_SIZE));
  }
}

//a tournaments of several matchs against two players
void tournament(int nb_match, enum colors (*p1)(), enum colors (*p2)()) {
  int nb_win1 = 0;
  int nb_win2 = 0;
  printf("Leeeeeeeet's go for a wonderful tournament!\n");
  for (int i = 0 ; i < nb_match ; i++) {
    board_game board = init_board();
    full_game(*p1, *p2, &board, 0);

    if (board.nb_player1 > board.nb_player2) {
      nb_win1++;
    } else {
      nb_win2++;
    }
  }
  printf("Here come the results...");
  printf("Player 1 has win %d match(s) whereas Player 2 has won %d!\n", nb_win1, nb_win2);
  printf("But don't forget than everyone is a winner :)\n");
}

/**************************************************************/
//                         Players                            //
/**************************************************************/

//player who randomly chooses a color each turn
enum colors player_random(board_game *board) {
  int color_index = rand() % NB_COLORS;
  enum colors color = A;
  color += color_index;
  return color;

}

//pretty self explanatory
enum colors player_human(board_game *board) {
  int color_index = 0;
  do {
    printf("Enter a number between 1 and 7 (for A to G) : ");
    scanf("%d", &color_index);
  } while (!(color_index > 0 && color_index <= 7));
  enum colors color = A;
  return color + (color_index - 1);
}

//set the color in x + dx, y + dy as seen
//used in set_colors_availables
void add_adjacent_colors(int x, int y, int dx, int dy, int *colors_available, board_game *board) {
  if (cell_in_bound(x + dx, y + dy)) { //checking if the position isn't out of the board
    int i = colors_to_int(get_cell(x + dx, y + dy, board));
    colors_available[i] = 1;
  }
}

//fill the tab of the available colors at this turn
void set_colors_availables(int *colors_available, board_game *board) {
  for (int y = 0 ; y < BOARD_SIZE ; y++) {    //we keep track of which colors whe can access and which we can't
    for (int x = 0 ; x < BOARD_SIZE ; x++) {
      if (get_cell(x, y, board) == board->turn) {
        add_adjacent_colors(x, y, -1, 0, colors_available, board);
        add_adjacent_colors(x, y, 1, 0, colors_available, board);
        add_adjacent_colors(x, y, 0, -1, colors_available, board);
        add_adjacent_colors(x, y, 0, 1, colors_available, board);
      }
    }
  }
}

//player who randomly chooses a color among the ones that he can reach
enum colors player_smart_random(board_game *board) {
  int colors_available[9] = {0};
  int nb_colors = 0;
  set_colors_availables(colors_available, board);
  for (int i = 0 ; i < 7 ; i++) { //searching for the number of colors we can access
    if (colors_available[i]) {
      nb_colors++;
    }
  }
  if (nb_colors == 0) { //we can't acess any color (the other player is blocking the way), so any color will have no effect
    enum colors res = A;
    return res;
  }
  else {

    int color_number = rand() % nb_colors + 1;
    int index = 0;
    int colors_seen = 0;
    enum colors color = A;
    while (colors_seen < color_number) {  //searching for the index of the color_indexth we can access
      if (colors_available[index]) {
        colors_seen++;
      }
      index++;
    }
    return color + (index - 1);

  }
}

//player who chooses the color that makes him conquer the most positions
enum colors player_greedy(board_game *board) {
  //Player plays the color that'll add the maximum of cells to his territory
  int best_index = 0;
  int max_nb_cell_conquered = 0;

  for (int i = 0 ; i < NB_COLORS ; i ++) {
    enum colors base_color = A;
    enum colors tab_copied[BOARD_SIZE * BOARD_SIZE];
    copy_game(tab_copied, board);
    board_game new_board = {tab_copied, board->turn, board->nb_player1, board->nb_player2};
    update_board_dfs(base_color + i, &new_board);
    int cells_conquered = new_board.nb_player1 - board->nb_player1 + new_board.nb_player2 - board->nb_player2;
    if (cells_conquered >= max_nb_cell_conquered) {
      best_index = i;
      max_nb_cell_conquered = cells_conquered;
    }
  }
  enum colors color = A;
  return color + best_index;
}

//player who chooses the first color of the 2 colors combinaison that makes him conquer the most positions
enum colors player_greedy_2_turns(board_game *board) {
  //int nb_conquered[NB_COLORS][NB_COLORS] = {0};
  int colors_available[9] = {0};
  set_colors_availables(colors_available, board);
  int best_index = 0;
  int max_nb_cell_conquered = 0;
  for (int i = 0 ; i < NB_COLORS ; i ++) {
    if (colors_available[i]) {

      enum colors base_color = A;
      enum colors tab_copied[BOARD_SIZE * BOARD_SIZE];
      copy_game(tab_copied, board);
      board_game new_board = {tab_copied, board->turn, board->nb_player1, board->nb_player2};
      update_board_dfs(base_color + i, &new_board);
      new_board.turn = board->turn; //Don not forget this, otherwhise he'll play for the other player
      update_board_dfs(player_greedy(&new_board), &new_board); //this is the only difference from the greedy player
      int cells_conquered = new_board.nb_player1 - board->nb_player1 + new_board.nb_player2 - board->nb_player2; //one of both parts is 0
      if (cells_conquered > max_nb_cell_conquered) {
        best_index = i;
        max_nb_cell_conquered = cells_conquered;
      }
    }
  }
  enum colors color = A;
  return color + best_index;
}

//player who chooses the move that increases the most the perimeter of its field
enum colors player_hegemonic(board_game *board) {
  //a player who emphasizes on maximizing its territory's perimeter
  int colors_available[9] = {0};
  set_colors_availables(colors_available, board);

  int best_index = 0;
  int max_reachable_perimeter = 0;
  for (int i = 0 ; i < NB_COLORS ; i ++) {
    if (colors_available[i]) {
      enum colors base_color = A;
      enum colors tab_copied[BOARD_SIZE * BOARD_SIZE] = {0};
      copy_game(tab_copied, board);
      board_game new_board = {tab_copied, board->turn, board->nb_player1, board->nb_player2};
      update_board_dfs(base_color + i, &new_board);

      int new_perimeter = perimeter(&new_board, board->turn);
      if (new_perimeter >= max_reachable_perimeter) {
        best_index = i;
        max_reachable_perimeter = new_perimeter;
      }
    }

  }
  enum colors color = A;
  return color + best_index;
}

/** Program entry point */
int main(void) {
    printf("\n\nWelcome to the 7 wonders of the world of the 7 colors\n"
	   "*****************************************************\n\n"
	   "Current board state:\n");

    srand(time(NULL)); //initialization of the random sequence
    /*board_game board = init_board();
    print_board(&board);
    full_game(player_random, player_hegemonic, &board, 1);*/
    tournament(480, player_greedy_2_turns, player_hegemonic);
    return 0; // Everything went well
}
