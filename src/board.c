#include "othello/board.h"

#include <bits/stdint-uintn.h>
#include <stdio.h>
#include <stdlib.h>

struct _board {
  uint8_t width;
  uint8_t height;
  uint16_t size;
  uint8_t *data;
};

board_t *board_create(uint8_t width, uint8_t height) {
  board_t *board = 0;
  uint16_t i = 0;

  board = (board_t *)malloc(sizeof(board_t));
  if (!board)
    return 0;

  board->width = width;
  board->height = height;
  board->size = board->width * board->height;

  board->data = (uint8_t *)malloc(board->size * sizeof(uint8_t));
  if (!board->data) {
    board_free(&board);
    return 0;
  }

  for (i = 0; i < board->size; ++i)
    board->data[i] = 0;

  return board;
}

void board_init(board_t *board, uint8_t row, uint8_t col) {
  if (!board || !board->data || row <= 0 || row >= board->height || col <= 0 ||
      col >= board->width)
    return;

  board->data[(row - 1) * board->width + (col - 1)] = 1;
  board->data[(row - 1) * board->width + (col - 0)] = 2;
  board->data[(row - 0) * board->width + (col - 1)] = 2;
  board->data[(row - 0) * board->width + (col - 0)] = 1;
}

void board_free(board_t **board) {
  if (!board || !*board)
    return;

  free(*board);
  *board = 0;
}

void board_draw(board_t *board) {
  uint16_t i = 0;

  if (!board || !board->data)
    return;

  for (i = 0; i < board->size; ++i) {
    printf("%s%s",
           board->data[i] == 1
               ? STONE_WHITE
               : (board->data[i] == 2 ? STONE_BLACK : STONE_NONE),
           (i + 1) % board->width ? "" : "\n");
  }
}

void board_moves_calculate(board_t *board) {
  uint8_t x = 0;
  uint8_t y = 0;

  for (y = 0; y < board->height; ++y) {
    for (x = 0; x < board->width; ++x) {
      /*
       * TODO:
       * Check 3x3 Area around the cell
       * When an enemy stone is hit, start checking in its direction
       * if a blank cell or the edge of the board is hit, move is not valid
       * else if friendly stone is hit, move is valid
       */
    }
  }
}
