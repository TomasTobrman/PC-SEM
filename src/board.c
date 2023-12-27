#include "othello/board.h"
#include "othello/player.h"

#include <bits/stdint-intn.h>
#include <bits/stdint-uintn.h>
#include <stdio.h>
#include <stdlib.h>

struct _board {
  uint8_t width;
  uint8_t height;
  uint16_t size;
  player_color *data;
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

  board->data = (player_color *)malloc(board->size * sizeof(player_color));
  if (!board->data) {
    board_free(&board);
    return 0;
  }

  for (i = 0; i < board->size; ++i)
    board->data[i] = PLAYER_COLOR_NONE;

  return board;
}

void board_init(board_t *board, uint8_t row, uint8_t col) {
  if (!board || !board->data || row <= 0 || row >= board->height || col <= 0 ||
      col >= board->width)
    return;

  board->data[(row - 1) * board->width + (col - 1)] = PLAYER_COLOR_WHITE;
  board->data[(row - 1) * board->width + (col - 0)] = PLAYER_COLOR_BLACK;
  board->data[(row - 0) * board->width + (col - 1)] = PLAYER_COLOR_BLACK;
  board->data[(row - 0) * board->width + (col - 0)] = PLAYER_COLOR_WHITE;
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

void board_calculate(board_t *board, player_color color) {
  uint8_t x = 0;
  uint8_t y = 0;
  uint8_t ox = 0;
  uint8_t oy = 0;
  uint8_t dir = 0;
  uint16_t i = 0;

  int8_t offset_x[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
  int8_t offset_y[8] = {-1, -1, -1, 0, 0, 1, 1, 1};

  for (y = 0; y < board->height; ++y) {
    for (x = 0; x < board->width; ++x) {
      /* skip cell if occupied */
      if (board->data[y * board->width + x])
        continue;

      /* check 3x3 area around cell */
      /* dir = ↖ ↑ ↗ ← → ↙ ↓ ↘ */
      for (dir = 0; dir < 8; ++dir) {
        ox = x + offset_x[i];
        oy = y + offset_y[i];

        /* out of bounds */
        if (ox < 0 || ox >= board->width || oy < 0 || oy >= board->height)
          continue;

        i = oy * board->width + ox;
        if (board->data[i] != color) {
          /* TODO: continue in the direction */
        }
      }
    }
  }
}
