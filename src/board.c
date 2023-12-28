#include "othello/board.h"
#include "othello/player.h"

#include <stdio.h>
#include <stdlib.h>

struct _board {
    uint8_t width;
    uint8_t height;
    uint16_t size;
    player_color *data;
    uint16_t *moves;
};

board_t *board_create(uint8_t width, uint8_t height) {
    board_t *board = 0;
    uint16_t i     = 0;

    board = (board_t *)malloc(sizeof(board_t));
    if (!board) return 0;

    board->width  = width;
    board->height = height;
    board->size   = board->width * board->height;

    board->data = (player_color *)malloc(board->size * sizeof(player_color));
    if (!board->data) {
        board_free(&board);
        return 0;
    }

    board->moves = (uint16_t *)malloc(board->size * sizeof(uint16_t));
    if (!(board->moves)) {
        board_free(&board);
        return 0;
    }

    for (i = 0; i < board->size; ++i) {
        board->data[i]  = PLAYER_COLOR_NONE;
        board->moves[i] = 0;
    }

    return board;
}

void board_init(board_t *board, uint8_t row, uint8_t col) {
    if (!board || !board->data || row <= 0 || row >= board->height ||
        col <= 0 || col >= board->width)
        return;

    board->data[(row - 1) * board->width + (col - 1)] = PLAYER_COLOR_WHITE;
    board->data[(row - 1) * board->width + (col - 0)] = PLAYER_COLOR_BLACK;
    board->data[(row - 0) * board->width + (col - 1)] = PLAYER_COLOR_BLACK;
    board->data[(row - 0) * board->width + (col - 0)] = PLAYER_COLOR_WHITE;
}

void board_free(board_t **board) {
    if (!board || !*board) return;
    if ((*board)->moves) free((*board)->moves);
    if ((*board)->data) free((*board)->data);
    free(*board);
    *board = 0;
}

void board_draw(board_t *board) {
    uint16_t i = 0;

    if (!board || !board->data) return;

    for (i = 0; i < board->size; ++i) {
        printf("%s%s",
               board->data[i] == 1
                   ? STONE_WHITE
                   : (board->data[i] == 2 ? STONE_BLACK : STONE_NONE),
               (i + 1) % board->width ? "" : "\n");
    }
}

void board_draw_moves(board_t *board) {
    uint16_t i = 0;

    if (!board || !board->data) return;

    for (i = 0; i < board->size; ++i) {
        printf(" %d %s", board->moves[i], (i + 1) % board->width ? "" : "\n");
    }
}

static uint16_t check_direction(board_t *board, player_color color, int16_t x,
                                int16_t y, int8_t dir_x, int8_t dir_y) {
    uint16_t count = 0;
    uint16_t i     = 0;
    int16_t ox     = x;
    int16_t oy     = y;

    for (;;) {
        ox += dir_x;
        oy += dir_y;

        /* out of bounds */
        if (ox < 0 || ox >= board->width || oy < 0 || oy >= board->height) {
            return 0;
        }

        /* get index from 2D coordinates */
        i = oy * board->width + ox;

        /* found blank space */
        if (board->data[i] == PLAYER_COLOR_NONE) {
            return 0;
        }

        /* move is valid */
        if (board->data[i] == color) {
            return count;
        }

        ++count;
    }
}

static uint16_t check_3x3(board_t *board, player_color color, int16_t x,
                          int16_t y) {
    int16_t ox      = 0;
    int16_t oy      = 0;
    int16_t i       = 0;
    int8_t dir      = 0;
    int8_t dir_x[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
    int8_t dir_y[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    uint16_t weight = 0;

    /* dir = ↖ ↑ ↗ ← → ↙ ↓ ↘ */
    for (dir = 0; dir < 8; ++dir) {
        ox = x + dir_x[dir];
        oy = y + dir_y[dir];

        /* out of bounds */
        if (ox < 0 || ox >= board->width || oy < 0 || oy >= board->height)
            continue;

        /* get index from 2D coordinates */
        i = oy * board->width + ox;

        /* found enemy stone, check in their direction */
        if (board->data[i] != color)
            weight +=
                check_direction(board, color, x, y, dir_x[dir], dir_y[dir]);
    }

    return weight;
}

void board_calculate(board_t *board, player_color color) {
    uint8_t x  = 0;
    uint8_t y  = 0;
    uint16_t i = 0;

    if (color != PLAYER_COLOR_WHITE && color != PLAYER_COLOR_BLACK) return;

    for (y = 0; y < board->height; ++y) {
        for (x = 0; x < board->width; ++x) {
            /* get index from 2D coordinates */
            i = y * board->width + x;

            /* skip cell if occupied */
            if (board->data[i]) {
                board->moves[i] = 0;
                continue;
            }

            /* check 3x3 area around cell */
            board->moves[i] = check_3x3(board, color, x, y);
        }
    }
}
