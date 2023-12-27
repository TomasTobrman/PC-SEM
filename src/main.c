#include "othello/board.h"
#include <bits/stdint-uintn.h>
#include <stdio.h>
#include <string.h>

#define CMD_SAVE "save"
#define CMD_QUIT "quit"
#define CMD_PASS "pass"
#define CMD_MOVE_SEPARATOR "_"

#define INPUT_SIZE 256

/* FIX: Error checking EVERYWHERE */
int main(int argc, char *argv[]) {
  int exit_status = 0;
  board_t *board = 0;
  player_t cur_player = PLAYER_NONE;
  char input[INPUT_SIZE];

  uint16_t i = 0;
  for (i = 0; i < INPUT_SIZE; ++i)
    input[i] = 0;

  board = board_create(8, 8);
  if (!board)
    return -1;

  board_init(board, 4, 4);

  for (;;) {
    /* switch current player */
    cur_player = cur_player == PLAYER_WHITE ? PLAYER_BLACK : PLAYER_WHITE;

    board_draw(board);

    fgets(input, INPUT_SIZE, stdin);
    /* remove trailing '\n' */
    input[strcspn(input, "\r\n")] = 0;

    /* quit command */
    if (!strcmp(input, CMD_QUIT)) {
      printf("quitting...\n");
      exit_status = 0;
      break;
    }

    /* pass command */
    if (!strcmp(input, CMD_PASS)) {
      printf("%s player passed\n",
             cur_player == PLAYER_WHITE ? "white" : "black");
      continue;
    }
  }

  board_free(&board);
  return exit_status;
}
