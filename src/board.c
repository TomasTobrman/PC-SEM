#include "othello/board.h"
#include "othello/types.h"

#include <stdio.h>
#include <stdlib.h>

#define STONE_NONE  "·"
#define STONE_BLACK "○"
#define STONE_WHITE "◙"

/**
 * Create a new board
 *
 * @param unsigned int width Number of columns of the board
 * @param unsigned int height Number of rows of the board
 * @return board_t * Structure pointer to the allocated board, NULL when an
 * error occurs
 */
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

/**
 * Make a copy of a board_t structure
 *
 * @param const board_t * original The original board structure to copy
 * @return board_t * Copy of the original board_t structure
 */
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

/**
 * Place the initial stones to the board
 *
 * @param board_t * board A board_t structure
 * @param unsigned int col Column of the top left stone
 * @param unsigned int row Row of the top left stone
 * @return bool Returns true if stones could be placed, else returns false
 */
bool board_init(const board_t *board, unsigned int col, unsigned int row) {
    if (!board || !board->data || row <= 0 || row >= board->height ||
        col <= 0 || col >= board->width)
        return false;

    board->data[(row - 1) * board->width + (col - 1)] = PLAYER_COLOR_WHITE;
    board->data[(row - 1) * board->width + (col - 0)] = PLAYER_COLOR_BLACK;
    board->data[(row - 0) * board->width + (col - 1)] = PLAYER_COLOR_BLACK;
    board->data[(row - 0) * board->width + (col - 0)] = PLAYER_COLOR_WHITE;

    return true;
}

/**
 * Free the board_t struct
 *
 * @param board_t **board Address of a board_t pointer
 */
void board_free(board_t **board) {
    if (!board || !*board) return;
    if ((*board)->moves) free((*board)->moves);
    if ((*board)->data) free((*board)->data);
    free(*board);
    *board = NULL;
}

/**
 * Draw the board to stdout
 *
 * @param const board_t * board A board_t struct width data to draw
 */
void board_draw(const board_t *board) {
    unsigned int i = 0;

    if (!board || !board->data) return;

    for (i = 0; i < board->size; ++i) {
        switch (board->data[i]) {
            case PLAYER_COLOR_BLACK:
                printf(" %s ", STONE_BLACK);
                break;
            case PLAYER_COLOR_WHITE:
                printf(" %s ", STONE_WHITE);
                break;
            default:
                printf(" %s ", STONE_NONE);
                break;
        }
        if (!((i + 1) % board->width)) printf("\n");
    }
}

/**
 * Check available moves in a direction
 *
 * @param const board_t * board Board structure pointer with game data
 * @param player_color color Player for which to check available moves
 * @param unsigned int col Column of the checked cell
 * @param unsigned int row Row of the checked cell
 * @param int dir_col Column direction in which to check
 * @param int dir_row Row direction in which to check
 * @param bool flip If stones should be flipped on found move
 * @return int Count of stones to be flipped
 */
static int check_direction(const board_t *board, player_color color,
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
        if (ocol < 0 || ocol >= board->width || orow < 0 ||
            orow >= board->height) {
            return 0;
        }

        /* get index from 2D coordinates */
        i = orow * board->width + ocol;

        /* found blank space */
        if (board->data[i] == PLAYER_COLOR_NONE) return 0;

        /* move is valid */
        if (board->data[i] == color) {
            for (; flip;) {
                if (ocol == col && orow == row) break;
                board->data[orow * board->width + ocol] = color;
                ocol -= dir_col;
                orow -= dir_row;
            }

            return count;
        }

        ++count;
    }
}

/**
 * Check available moves around a cell
 *
 * @param const board_t * board Board structure pointer with game data
 * @param player_color color Player for which to check available moves
 * @param unsigned int col Column of the checked cell
 * @param unsigned int row Row of the checked cell
 * @param bool flip If stones should be flipped on found move
 * @return int Count of stones to be flipped in all directions
 */
static int check_3x3(const board_t *board, player_color color, unsigned int col,
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
        if (ocol < 0 || ocol >= board->width || orow < 0 ||
            orow >= board->height)
            continue;

        /* get index from 2D coordinates */
        i = orow * board->width + ocol;

        /* found enemy stone, check in their direction */
        if (board->data[i] != color)
            weight += check_direction(board, color, col, row, dir_col[dir],
                                      dir_row[dir], flip);
    }

    return weight;
}

/**
 * Calculate available moves for a player
 *
 * @param const board_t * board Board structure with game data
 * @param player_color color The player for which moves should be calculated
 */
void board_calculate(const board_t *board, player_color color) {
    unsigned int col = 0;
    unsigned int row = 0;
    unsigned int i   = 0;

    if (!board || !board->data || !board->moves ||
        (color != PLAYER_COLOR_WHITE && color != PLAYER_COLOR_BLACK))
        return;

    for (row = 0; row < board->height; ++row) {
        for (col = 0; col < board->width; ++col) {
            /* get index from 2D coordinates */
            i = row * board->width + col;

            /* skip cell if occupied */
            if (board->data[i]) {
                board->moves[i] = 0;
                continue;
            }

            /* check 3x3 area around cell */
            board->moves[i] = check_3x3(board, color, col, row, false);
        }
    }
}

/**
 * Play a move on the board
 *
 * @param const board_t * board Board structure with game data
 * @param player_color color Player which is playing the move
 * @param unsigned int col Played column
 * @param unsigned int row Played row
 * @return bool Returns true if the move was played, else returns false
 */
bool board_play(const board_t *board, player_color color, unsigned int col,
                unsigned int row) {
    int i = 0;

    if (!board || !board->data || !board->moves) return false;

    col -= 1;
    row -= 1;

    if (col >= board->width || row >= board->height) return false;

    i = row * board->width + col;
    if (!board->moves[i]) return false;

    check_3x3(board, color, col, row, true);

    board->data[i] = color;

    return true;
}

/**
 * Does the board have available calculated moves
 *
 * @param const board_t * board Board structure pointer with game data
 * @return bool Returns true if there are moves available, else returns false
 */
bool board_can_play(const board_t *board) {
    int i = 0;
    if (!board || !board->moves) return false;
    for (i = 0; i < board->size; ++i)
        if (board->moves[i]) return true;
    return false;
}

/**
 * Is the board filled with stones
 *
 * @param const board_t * board Board structure pointer with game data
 * @return bool Returns true if the board is filled, else returns false
 */
bool board_is_full(const board_t *board) {
    int i = 0;
    for (i = 0; i < board->size; ++i)
        if (board->data[i] == PLAYER_COLOR_NONE) return false;
    return true;
}

/**
 * Number of stones of a color
 *
 * @param const board_t * board Board structure pointer with game data
 * @param player_color color Color of the stones to count
 * @return Count of stones on the board of the color
 */
int board_count(const board_t *board, player_color color) {
    int i   = 0;
    int sum = 0;
    if (!board || !board->data ||
        (color != PLAYER_COLOR_WHITE && color != PLAYER_COLOR_BLACK))
        return 0;
    for (i = 0; i < board->size; ++i) sum += board->data[i] == color ? 1 : 0;
    return sum;
}
