/* Template of the 7 wonders of the world of the 7 colors assigment. */

#include <stdio.h>     /* printf */
#include <stdlib.h>     //random
#include <time.h>     //to initialize random

/* We want a 30x30 board game by default */
#define BOARD_SIZE 30
#define NB_COLORS 7 // 7 colors (+ 2 players)
#define MAX(a, b) ((a)<(b)? (b) : (a)) //max between two numbers

//Allows us to transfor an int into a color
/*TODO (maybe) -> do all this in a struct/enum*/
char color_translator[] = "ABCDEFG^v";


/** Represent the actual current board game
 *
 * NOTE: global variables are usually discouraged (plus encapsulation in
 *     an appropriate data structure would also be preferred), but don't worry.
 *     For this first assignment, no dinosaure will get you if you do that.
 */
char board[BOARD_SIZE * BOARD_SIZE] = { 0 }; // Filled with zeros




/** Retrieves the color of a given board cell */
char get_cell(int x, int y)
{
    return board[y * BOARD_SIZE + x];
}

/** Changes the color of a given board cell */
void set_cell(int x, int y, char color)
{
    board[y * BOARD_SIZE + x] = color;
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
            printf("%c", get_cell(i, j));
        }
        printf("\n");
    }
}


void init_board() { //initialize the board by picking a random color for each position
  for (int x = 0 ; x < BOARD_SIZE ; x++) {
    for (int y = 0 ; y < BOARD_SIZE ; y++) {
      int color_index = rand() % NB_COLORS;
      char color = color_translator[color_index];
      set_cell(x, y, color);
    }
  }
  set_cell(0, BOARD_SIZE - 1, color_translator[7]); //first player
  set_cell(BOARD_SIZE - 1, 0, color_translator[8]); //second player
}

int update_around_cell (int x, int y, char player, char color_played) { //returns 1 if something has changed, 0 otherwhise
  int has_done_something = 0;


  if (get_cell(x, y) == player) {
    if (x > 0 && get_cell(x - 1, y) == color_played) { //position on top
      set_cell(x - 1, y, player);
      has_done_something = 1;
    }
    if (x < BOARD_SIZE - 1 && get_cell(x + 1, y) == color_played) { //position under
      set_cell(x + 1, y, player);
      has_done_something = 1;
    }
    if (y > 0 && get_cell(x, y - 1) == color_played) { //position on the left
      set_cell(x, y - 1, player);
      has_done_something = 1;
    }
    if (y < BOARD_SIZE - 1 && get_cell(x, y + 1) == color_played) { //position on the right
      set_cell(x, y + 1, player);
      has_done_something = 1;
    }
  }
  return has_done_something;
}

void update_board(char player, char color_played) { //player = "^" ou "v" et color_played = A...G
  int has_done_something;
  for (int x = 0 ; x < BOARD_SIZE ; x++) {
    for (int y = 0 ; y < BOARD_SIZE ; y++) {
      has_done_something = MAX(1, update_around_cell(x, y, player, color_played));
    }
  }
}

char random_color() {
  int color_index = rand() % NB_COLORS;
  return color_translator[color_index];
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
    printf("\n\n");
    update_board(color_translator[7], random_color());
    print_board();
    printf("\n\n");
    update_board(color_translator[8], random_color());
    print_board();


    return 0; // Everything went well
}
