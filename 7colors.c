/* Template of the 7 wonders of the world of the 7 colors assigment. */

#include <stdio.h>     /* printf */
#include <stdlib.h>     //random
#include <time.h>     //to initialize random

//TODO : include BOARD_SIZE and NB_COLORS into the struct

/* We want a 30x30 board game by default */
#define BOARD_SIZE 30
#define NB_COLORS 7 // 7 colors (+ 2 players)
#define MAX(a, b) ((a)<(b)? (b) : (a)) //max between two numbers




enum colors {
  A, B, C, D, E, F, G, PLAYER1, PLAYER2
};

typedef struct board_game board_game;

struct board_game {
  char* game;       //the board game
  enum colors turn; //the player who has to play next
  int unused;       //the number of positions which doesn't belong to any player
};





/** Represent the actual current board game
 *
 * NOTE: global variables are usually discouraged (plus encapsulation in
 *     an appropriate data structure would also be preferred), but don't worry.
 *     For this first assignment, no dinosaure will get you if you do that.
 */

char tab[BOARD_SIZE * BOARD_SIZE] = { 0 }; // Filled with zeros
board_game board = {tab, PLAYER1, BOARD_SIZE * BOARD_SIZE - 2};




/** Retrieves the color of a given board cell */
enum colors get_cell(int x, int y)
{
    return tab[y * BOARD_SIZE + x];
}

/** Changes the color of a given board cell */
void set_cell(int x, int y, enum colors color)
{
    tab[y * BOARD_SIZE + x] = color;
}

//Allows us to transform a color into its char representation
char color_translator(enum colors color) {
  char translator[9] = "ABCDEFG^v";
  switch(color) {       //TODO (maybe) : use a smarter way using pointer arithmetic and ascii representation
      case A:
        return translator[0];
        break;
      case B:
        return translator[1];
        break;
      case C:
        return translator[2];
        break;
      case D:
        return translator[3];
        break;
      case E:
        return translator[4];
        break;
      case F:
        return translator[5];
        break;
      case G:
        return translator[6];
        break;
      case PLAYER1:
        return translator[7];
        break;
      case PLAYER2:
        return translator[8];
        break;
      default:
        return 0;
  }
}

/** Prints the current state of the board on screen
 *
 * Implementation note: It would be nicer to do this with ncurse or even
 * SDL/allegro, but this is not really the purpose of this assignment.
 */
void print_board(void)
{
    int i, j;
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
          enum colors color =  get_cell(i, j);
          printf("%c", color_translator(color));
        }
        printf("\n");
    }
}

void init_board() { //initialize the board by picking a random color for each position
  for (int x = 0 ; x < BOARD_SIZE ; x++) {
    for (int y = 0 ; y < BOARD_SIZE ; y++) {
      int color_index = rand() % NB_COLORS;
      enum colors color = A;
      color += color_index;
      set_cell(x, y, color);
    }
  }
  set_cell(0, BOARD_SIZE - 1, PLAYER1); //first player
  set_cell(BOARD_SIZE - 1, 0, PLAYER2); //second player
}

//update one position next to (x, y) in the direction (dx, dy) where dx + dy = 1, dxdy = 0
void update_one_dir(int x, int y, int dx, int dy, enum colors color_played, int* has_done_something) {
  if (x + dx < BOARD_SIZE && x + dx >= 0 && y + dy < BOARD_SIZE && y + dy >= 0 && get_cell(x + dx, y + dy) == color_played) { //checking if the position isn't out of the board
    set_cell(x + dx, y + dy, board.turn);
    *has_done_something--;
    board.unused--;
  }
}

int update_around_cell (int x, int y, enum colors color_played) { //returns 1 if something has changed, 0 otherwhise
  int has_done_something = 0;
  if (get_cell(x, y) == board.turn) {
    update_one_dir(x, y, -1, 0, color_played, &has_done_something);
    update_one_dir(x, y, 1, 0, color_played, &has_done_something);
    update_one_dir(x, y, 0, -1, color_played, &has_done_something);
    update_one_dir(x, y, 0, 1, color_played, &has_done_something);
  }
  return has_done_something;
}

void update_board(char color_played) { //player = "^" ou "v" et color_played = A...G
  int has_done_something = 1;
  while (has_done_something) {
    has_done_something = 0;
    for (int x = 0 ; x < BOARD_SIZE ; x++) {
      for (int y = 0 ; y < BOARD_SIZE ; y++) {
        has_done_something = MAX(has_done_something, update_around_cell(x, y, color_played));
      }
    }
  }
  if (board.turn == PLAYER1) {
    board.turn = PLAYER2;
  }
  else {
    board.turn = PLAYER1;
  }
}

enum colors random_color() {  //pretty self explanatory
  int color_index = rand() % NB_COLORS;
  enum colors color = A;
  color += color_index;
  return color;

}

void full_game_random() {//entire game where each player chooses a random color each turn
  int k = 0;
  while(board.unused > 0) {
    printf("\n\n");
    enum colors color = random_color();
    printf("player %d played color %c\n", k + 1, color_translator(color));
    update_board(color);
    print_board();
    printf("%d positions left\n", board.unused);
    k = (k + 1) % 2;
  }
}

void decide_winner() { //we assume the game is over, ie board.unused == 0
  int nb_player1 = 0;
  int nb_player2 = 0;
  for (int x = 0 ; x < BOARD_SIZE ; x++) {
    for (int y = 0 ; y < BOARD_SIZE ; y++) {
      if (get_cell(x, y) == PLAYER1) {
        nb_player1++;
      }
      else {
        nb_player2++;
      }
    }
  }
  if (nb_player1 > nb_player2) {
    printf("Player 1 won the game! with %d percents of the board\n", 100 * nb_player1 / (BOARD_SIZE * BOARD_SIZE));
  }
  else {
    printf("Player 2 won the game! with %d percents of the board\n", 100 * nb_player2 / (BOARD_SIZE * BOARD_SIZE));
  }
}

/** Program entry point */
int main(void)
{
    printf("\n\nWelcome to the 7 wonders of the world of the 7 colors\n"
	   "*****************************************************\n\n"
	   "Current board state:\n");

    srand(time(NULL)); //initialization of the random sequence
    init_board();
    print_board();
    full_game_random();
    decide_winner();
    return 0; // Everything went well
}
