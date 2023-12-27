#include "othello/board.h"

/* FIX: Error checking EVERYWHERE */
int main(int argc, char *argv[]) {
  board_t *board = 0;

  board = board_create(8, 8);

  board_init(board, 4, 4);

  board_draw(board);

  board_play(board, PLAYER_WHITE, 3, 5);

  board_draw(board);

  board_free(&board);

  return 0;
}
