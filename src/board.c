#include "othello/board.h"

#include <bits/stdint-uintn.h>
#include <stdio.h>
#include <stdlib.h>

struct _board {
  uint8_t width;
  uint8_t height;
  uint16_t size;
  player_t *data;
  uint16_t *moves_white;
  uint16_t *moves_black;
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

  board->data = (player_t *)malloc(board->size * sizeof(player_t));
  if (!board->data) {
    board_free(&board);
    return 0;
  }

  board->moves_white = (uint16_t *)malloc(board->size * sizeof(uint16_t));
  if (!board->moves_white) {
    board_free(&board);
    return 0;
  }

  board->moves_black = (uint16_t *)malloc(board->size * sizeof(uint16_t));
  if (!board->moves_black) {
    board_free(&board);
    return 0;
  }

  for (i = 0; i < board->size; ++i) {
    board->data[i] = PLAYER_NONE;
    board->moves_white[i] = 0;
    board->moves_black[i] = 0;
  }

  return board;
}

void board_init(board_t *board, uint8_t row, uint8_t col) {
  if (!board || !board->data || row <= 0 || row > board->height || col <= 0 ||
      col > board->width)
    return;

  board->data[(row - 1) * board->width + (col - 1)] = PLAYER_WHITE;
  board->data[(row - 1) * board->width + (col - 0)] = PLAYER_BLACK;
  board->data[(row - 0) * board->width + (col - 1)] = PLAYER_BLACK;
  board->data[(row - 0) * board->width + (col - 0)] = PLAYER_WHITE;
}

void board_free(board_t **board) {
  if (!board || !*board)
    return;

  if ((*board)->data)
    free((*board)->data);

  if ((*board)->moves_white) {
    free((*board)->moves_white);
  }

  free(*board);
  *board = 0;
}

void board_draw(board_t *board) {
  uint16_t i = 0;

  if (!board || !board->data)
    return;

  for (i = 0; i < board->size; ++i) {
    printf("%s%s",
           board->data[i] == PLAYER_WHITE
               ? PIECE_WHITE
               : (board->data[i] == PLAYER_BLACK ? PIECE_BLACK : PIECE_BLANK),
           (i + 1) % board->width ? "" : "\n");
  }
}

void board_moves_calculate(board_t *board, player_t player) {
  uint8_t x = 0;
  uint8_t y = 0;

  if (!board || !board->data || !board->moves_white || !board->moves_black)
    return;

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

uint8_t board_play(board_t *board, player_t player, uint8_t row, uint8_t col) {
  uint16_t index = 0;

  if (!board || !board->data || row <= 0 || row > board->height || col <= 0 ||
      col > board->width)
    return 1;

  index = (row - 1) * board->width + (col - 1);

  switch (player) {
  case PLAYER_WHITE:
    if (board->moves_white[index])
      board->data[index] = player;
    break;
  case PLAYER_BLACK:
    if (board->moves_black[index])
      board->data[index] = player;
    break;
  default:
    return 1;
  }

  return 0;
}
