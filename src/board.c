#include "othello/board.h"
#include "othello/types.h"

#include <stdio.h>
#include <stdlib.h>

#define STONE_NONE  "·"
#define STONE_BLACK "○"
#define STONE_WHITE "◙"

board_t *board_copy(const board_t *original) {
    board_t *board = NULL;
    unsigned int i = 0;

    if (!original || !original->data || !original->moves) return NULL;

    board = board_create(original->width, original->height);
    if (!board) return NULL;

    for (i = 0; i < board->size; ++i) {
        board->data[i]  = original->data[i];
        board->moves[i] = original->moves[i];
    }

    return board;
}

board_t *board_create(unsigned int width, unsigned int height) {
    board_t *board = NULL;
    unsigned int i = 0;

    board = (board_t *)malloc(sizeof(board_t));
    if (!board) return NULL;

    board->width  = width;
    board->height = height;
    board->size   = board->width * board->height;
    board->data   = NULL;
    board->moves  = NULL;

    board->data = (player_color *)malloc(board->size * sizeof(player_color));
    if (!board->data) {
        board_free(&board);
        return NULL;
    }

    board->moves = (unsigned int *)malloc(board->size * sizeof(unsigned int));
    if (!board->moves) {
        board_free(&board);
        return NULL;
    }

    for (i = 0; i < board->size; ++i) {
        board->data[i]  = PLAYER_COLOR_NONE;
        board->moves[i] = 0;
    }

    return board;
}

bool board_init(board_t *board, unsigned int col, unsigned int row) {
    if (!board || !board->data || row <= 0 || row >= board->height ||
        col <= 0 || col >= board->width)
        return false;

    board->data[(row - 1) * board->width + (col - 1)] = PLAYER_COLOR_WHITE;
    board->data[(row - 1) * board->width + (col - 0)] = PLAYER_COLOR_BLACK;
    board->data[(row - 0) * board->width + (col - 1)] = PLAYER_COLOR_BLACK;
    board->data[(row - 0) * board->width + (col - 0)] = PLAYER_COLOR_WHITE;

    return true;
}

void board_free(board_t **board) {
    if (!board || !*board) return;
    if ((*board)->moves) free((*board)->moves);
    if ((*board)->data) free((*board)->data);
    free(*board);
    *board = NULL;
}

void board_draw(const board_t *board) {
    int i   = 0;
    int col = 0;
    int row = 0;

    if (!board || !board->data) return;

    for (row = -1; row < (int)board->height; ++row) {
        if (row >= 0) printf(" %d ", row + 1);
        else printf("   ");
        for (col = 0; col < (int)board->width; ++col) {
            if (row < 0) {
                printf(" %d ", col + 1);
                continue;
            }

            i = row * board->width + col;

            printf(" %s ",
                   board->data[i] == PLAYER_COLOR_WHITE
                       ? STONE_WHITE
                       : (board->data[i] == PLAYER_COLOR_BLACK ? STONE_BLACK
                                                               : STONE_NONE));
        }

        printf("\n");
    }
}

static int check_direction(board_t **board, player_color color,
                           unsigned int col, unsigned int row, int dir_col,
                           int dir_row, bool flip) {
    int i     = 0;
    int count = 0;
    int ocol  = col;
    int orow  = row;

    for (;;) {
        ocol += dir_col;
        orow += dir_row;

        /* out of bounds */
        if (ocol < 0 || ocol >= (*board)->width || orow < 0 ||
            orow >= (*board)->height) {
            return 0;
        }

        /* get index from 2D coordinates */
        i = orow * (*board)->width + ocol;

        /* found blank space */
        if ((*board)->data[i] == PLAYER_COLOR_NONE) return 0;

        /* move is valid */
        if ((*board)->data[i] == color) {
            for (; flip;) {
                if (ocol == col && orow == row) break;
                (*board)->data[orow * (*board)->width + ocol] = color;
                ocol -= dir_col;
                orow -= dir_row;
            }

            return count;
        }

        ++count;
    }
}

static int check_3x3(board_t **board, player_color color, unsigned int col,
                     unsigned int row, bool flip) {
    int i          = 0;
    int ocol       = 0;
    int orow       = 0;
    int dir        = 0;
    int dir_col[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
    int dir_row[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int weight     = 0;

    /* dir = ↖ ↑ ↗ ← → ↙ ↓ ↘ */
    for (dir = 0; dir < 8; ++dir) {
        ocol = col + dir_col[dir];
        orow = row + dir_row[dir];

        /* out of bounds */
        if (ocol < 0 || ocol >= (*board)->width || orow < 0 ||
            orow >= (*board)->height)
            continue;

        /* get index from 2D coordinates */
        i = orow * (*board)->width + ocol;

        /* found enemy stone, check in their direction */
        if ((*board)->data[i] != color)
            weight += check_direction(board, color, col, row, dir_col[dir],
                                      dir_row[dir], flip);
    }

    return weight;
}

void board_calculate(board_t **board, player_color color) {
    unsigned int col = 0;
    unsigned int row = 0;
    unsigned int i   = 0;

    if (!board || !*board || !(*board)->data || !(*board)->moves ||
        (color != PLAYER_COLOR_WHITE && color != PLAYER_COLOR_BLACK))
        return;

    for (row = 0; row < (*board)->height; ++row) {
        for (col = 0; col < (*board)->width; ++col) {
            /* get index from 2D coordinates */
            i = row * (*board)->width + col;

            /* skip cell if occupied */
            if ((*board)->data[i]) {
                (*board)->moves[i] = 0;
                continue;
            }

            /* check 3x3 area around cell */
            (*board)->moves[i] = check_3x3(board, color, col, row, false);
        }
    }
}

bool board_play(board_t **board, player_color color, unsigned int col,
                unsigned int row) {
    int i = 0;

    if (!board || !*board || !(*board)->data || !(*board)->moves) return false;

    col -= 1;
    row -= 1;

    if (col >= (*board)->width || row >= (*board)->height) return false;

    i = row * (*board)->width + col;
    if (!(*board)->moves[i]) return false;

    check_3x3(board, color, col, row, true);

    (*board)->data[i] = color;

    return true;
}

bool board_can_play(const board_t *board) {
    int i = 0;
    if (!board || !board->moves) return false;
    for (i = 0; i < board->size; ++i)
        if (board->moves[i]) return true;
    return false;
}

int board_count(const board_t *board, player_color color) {
    int i   = 0;
    int sum = 0;
    if (!board || !board->data ||
        (color != PLAYER_COLOR_WHITE && color != PLAYER_COLOR_BLACK))
        return 0;
    for (i = 0; i < board->size; ++i) sum += board->data[i] == color ? 1 : 0;
    return sum;
}

bool board_full(const board_t *board) {
    int i = 0;
    for (i = 0; i < board->size; ++i)
        if (board->data[i] == PLAYER_COLOR_NONE) return false;
    return true;
}
