#include "othello/minimax.h"
#include "othello/board.h"
#include "othello/types.h"

#include <i386/limits.h>
#include <stdio.h>
#include <stdlib.h>

int minimax(player_color color, const board_t *board, int depth, int alpha,
            int beta, bool maximizing, unsigned int *play_col,
            unsigned int *play_row) {
    int eval         = 0;
    int max_eval     = 0;
    int min_eval     = 0;
    int i            = 0;
    unsigned int col = 0;
    unsigned int row = 0;
    board_t *copy    = NULL;
    player_color other_color =
        color == PLAYER_COLOR_WHITE ? PLAYER_COLOR_BLACK : PLAYER_COLOR_WHITE;

    if (depth <= 0 || !board_can_play(board)) {
        if (color < 0) return -board_eval(board);
        return board_eval(board);
    }

    if (maximizing) {
        max_eval = INT_MIN;
        for (i = 0; i < board->size; ++i) {
            if (!board->moves[i]) continue;

            copy = board_copy(board);
            col  = (i % copy->width) + 1;
            row  = (i / copy->width) + 1;

            board_calculate(&copy, color);
            board_play(&copy, color, col, row);
            board_calculate(&copy, other_color);
            eval = minimax(color, copy, depth - 1, alpha, beta, false, 0, 0);
            board_free(&copy);

            if (eval > max_eval) {
                max_eval = eval;
                if (play_col) *play_col = col;
                if (play_row) *play_row = row;
            }

            alpha = alpha > eval ? alpha : eval;
            if (beta <= alpha) break;
        }
        return max_eval;
    } else {
        min_eval = INT_MAX;
        for (i = 0; i < board->size; ++i) {
            if (!board->moves[i]) continue;

            copy = board_copy(board);
            col  = (i % copy->width) + 1;
            row  = (i / copy->width) + 1;

            board_calculate(&copy, other_color);
            board_play(&copy, other_color, col, row);
            board_calculate(&copy, color);
            eval = minimax(color, copy, depth - 1, alpha, beta, true, 0, 0);
            board_free(&copy);

            if (eval < min_eval) {
                min_eval = eval;
                if (play_col) *play_col = col;
                if (play_row) *play_row = row;
            }

            beta = beta < eval ? beta : eval;
            if (beta <= alpha) break;
        }
        return min_eval;
    }

    return 0;
}
