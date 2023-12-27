#include "othello/board.h"
#include "othello/command.h"
#include "othello/othello.h"

#include <bits/stdint-uintn.h>
#include <stdio.h>

#define INPUT_SIZE 256

int main(int argc, char *argv[]) {
  /* command-line arguments */

  /* local variables */
  bool shouldExit = false;
  bool nextMove = true;
  board_t *board = 0;
  char command[INPUT_SIZE];

  /* initialize input buffer to zeros */
  uint16_t i = 0;
  for (i = 0; i < INPUT_SIZE; ++i)
    command[i] = 0;

  /* create a new board */
  board = board_create(8, 8);
  if (!board)
    return -1;

  /* place initial stones */
  board_init(board, 4, 4);

  while (!shouldExit) {
    /* switch players and redraw the board */
    if (nextMove) {
      /* TODO: keep current player */
      board_draw(board);
      nextMove = false;
    }

    /* get user input */
    fgets(command, INPUT_SIZE, stdin);

    /* interpret the given command */
    /* TODO: Player moves */
    switch (command_interpret(command)) {
    case CMD_PASS:
      nextMove = true;
      continue;
    case CMD_QUIT:
      shouldExit = true;
      break;
    case CMD_SAVE:
      printf("Invalid output file destination\n");
      break;
    default:
      printf("Invalid move\n");
    };
  }

  /* exit application */
  board_free(&board);
  return 0;
}
