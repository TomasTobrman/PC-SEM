#include "othello/board.h"
#include "othello/bool.h"
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

bool board_init(board_t *board, unsigned int x, unsigned int y) {
    if (!board || !board->data || y <= 0 || y >= board->height || x <= 0 ||
        x >= board->width)
        return false;

    board->data[(y - 1) * board->width + (x - 1)] = PLAYER_COLOR_WHITE;
    board->data[(y - 1) * board->width + (x - 0)] = PLAYER_COLOR_BLACK;
    board->data[(y - 0) * board->width + (x - 1)] = PLAYER_COLOR_BLACK;
    board->data[(y - 0) * board->width + (x - 0)] = PLAYER_COLOR_WHITE;

    return true;
}

void board_free(board_t **board) {
    if (!board || !*board) return;
    if ((*board)->moves) free((*board)->moves);
    if ((*board)->data) free((*board)->data);
    free(*board);
    *board = NULL;
}

void board_draw(board_t *board) {
    int i = 0;
    int x = 0;
    int y = 0;

    if (!board || !board->data) return;

    for (y = -1; y < (int)board->height; ++y) {
        if (y >= 0) printf(" %d ", y + 1);
        else printf("   ");
        for (x = 0; x < (int)board->width; ++x) {
            if (y < 0) {
                printf(" %d ", x + 1);
                continue;
            }

            i = y * board->width + x;

            printf(" %s ",
                   board->data[i] == PLAYER_COLOR_WHITE
                       ? STONE_WHITE
                       : (board->data[i] == PLAYER_COLOR_BLACK ? STONE_BLACK
                                                               : STONE_NONE));
        }

        printf("\n");
    }
}

static int check_direction(board_t *board, player_color color, unsigned int x,
                           unsigned int y, int dir_x, int dir_y, bool flip) {
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
            for (; flip;) {
                if (ox == x && oy == y) break;
                board->data[oy * board->width + ox] = color;
                ox -= dir_x;
                oy -= dir_y;
            }

            return count;
        }

        ++count;
    }
}

static int check_3x3(board_t *board, player_color color, unsigned int x,
                     unsigned int y, bool flip) {
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
            weight += check_direction(board, color, x, y, dir_x[dir],
                                      dir_y[dir], flip);
    }

    return weight;
}

void board_calculate(board_t *board, player_color color) {
    unsigned int x = 0;
    unsigned int y = 0;
    unsigned int i = 0;

    if (!board || !board->data || !board->moves ||
        (color != PLAYER_COLOR_WHITE && color != PLAYER_COLOR_BLACK))
        return;

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
            board->moves[i] = check_3x3(board, color, x, y, false);
        }
    }
}

bool board_play(board_t *board, player_color color, unsigned int x,
                unsigned int y) {
    int i = 0;

    if (!board || !board->data || !board->moves) return false;

    x -= 1;
    y -= 1;

    if (x < 0 || x >= board->width || y < 0 || y >= board->height) return false;

    i = y * board->width + x;
    if (!board->moves[i]) return false;

    check_3x3(board, color, x, y, true);

    board->data[i] = color;

    return true;
}

bool board_can_play(board_t *board) {
    int i = 0;
    if (!board || !board->moves) return false;
    for (i = 0; i < board->size; ++i)
        if (board->moves[i]) return true;
    return false;
}

int board_count(board_t *board, player_color color) {
    int i   = 0;
    int sum = 0;
    if (!board || !board->data ||
        (color != PLAYER_COLOR_WHITE && color != PLAYER_COLOR_BLACK))
        return 0;
    for (i = 0; i < board->size; ++i) sum += board->data[i] == color ? 1 : 0;
    return sum;
}
