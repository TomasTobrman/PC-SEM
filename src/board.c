#include "othello/board.h"
#include "othello/player.h"

#include <stdio.h>
#include <stdlib.h>

struct _board {
    unsigned int width;
    unsigned int height;
    unsigned int size;
    player_color *data;
    int *moves;
};

board_t *board_create(unsigned int width, unsigned int height) {
    board_t *board = 0;
    unsigned int i = 0;

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

    board->moves = (int *)malloc(board->size * sizeof(int));
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

void board_init(board_t *board, unsigned int x, unsigned int y) {
    if (!board || !board->data || y <= 0 || y >= board->height || x <= 0 ||
        x >= board->width)
        return;

    board->data[(y - 1) * board->width + (x - 1)] = PLAYER_COLOR_WHITE;
    board->data[(y - 1) * board->width + (x - 0)] = PLAYER_COLOR_BLACK;
    board->data[(y - 0) * board->width + (x - 1)] = PLAYER_COLOR_BLACK;
    board->data[(y - 0) * board->width + (x - 0)] = PLAYER_COLOR_WHITE;
}

void board_free(board_t **board) {
    if (!board || !*board) return;
    if ((*board)->moves) free((*board)->moves);
    if ((*board)->data) free((*board)->data);
    free(*board);
    *board = 0;
}

void board_draw(board_t *board) {
    unsigned int i = 0;

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
    unsigned int i = 0;

    if (!board || !board->data) return;

    for (i = 0; i < board->size; ++i) {
        printf(" %d %s", board->moves[i], (i + 1) % board->width ? "" : "\n");
    }
}

static int check_direction(board_t *board, player_color color, unsigned int x,
                           unsigned int y, int dir_x, int dir_y) {
    int count = 0;
    int i     = 0;
    int ox    = x;
    int oy    = y;

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

static int check_3x3(board_t *board, player_color color, unsigned int x,
                     unsigned int y) {
    int ox       = 0;
    int oy       = 0;
    int i        = 0;
    int dir      = 0;
    int dir_x[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
    int dir_y[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int weight   = 0;

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
    unsigned int x = 0;
    unsigned int y = 0;
    unsigned int i = 0;

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
