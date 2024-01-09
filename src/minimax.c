#include "othello/minimax.h"
#include "othello/board.h"

/**
 * Minimax algorithm with Alpha-Beta purging
 *
 * @param player_color color Player for which to calculate Minimax
 * @param board_t * board Board structure pointer with game data
 * @param int depth Max depth of the minimax algorithm
 * @param int alpha Alpha value of the Alpha-Beta pruning extension
 * @param int beta Beta value of the Alpha-Beta pruning extension
 * @param bool maximizing Is the calculated player the maximizing player
 * @param unsigned int * play_col Returned column of the best played move
 * @param unsigned int * play_row Returned row of the best played move
 * @return int Minimax evaluation
 */
int minimax(player_color color, board_t *board, int depth, int alpha, int beta,
            bool maximizing, unsigned int *play_col, unsigned int *play_row) {
    int eval         = 0;
    int ret_eval     = 0;
    int i            = 0;
    unsigned int col = 0;
    unsigned int row = 0;
    board_t *copy    = NULL;
    player_color other_color =
        color == PLAYER_COLOR_WHITE ? PLAYER_COLOR_BLACK : PLAYER_COLOR_WHITE;

    /* static evaluation */
    if (depth <= 0) {
        for (i = 0; i < board->size; ++i) {
            if (board->moves[i]) {
                if (play_col) *play_col = (i % board->width) + 1;
                if (play_row) *play_row = (i / board->width) + 1;
                break;
            }
        }
        eval = board_count(board, color) - board_count(board, other_color);
        return eval;
    };

    if (maximizing) { /* calculate evaluation for maximizing player */
        ret_eval = INT_MIN;
        for (i = 0; i < board->size; ++i) {
            if (!board->moves[i]) continue;

            copy = board_copy(board);
            col  = (i % copy->width) + 1;
            row  = (i / copy->width) + 1;

            board_calculate(copy, color);
            if (board_can_play(board)) board_play(copy, color, col, row);
            board_calculate(copy, other_color);
            eval = minimax(color, copy, depth - 1, alpha, beta, false, 0, 0);
            board_free(&copy);

            if (eval > ret_eval) {
                ret_eval = eval;
                if (play_col) *play_col = col;
                if (play_row) *play_row = row;
            }

            alpha = alpha > eval ? alpha : eval;
            if (beta <= alpha) break;
        }
    } else { /* calculate evaluation for minimazing player */
        ret_eval = INT_MAX;
        for (i = 0; i < board->size; ++i) {
            if (!board->moves[i]) continue;

            copy = board_copy(board);
            col  = (i % copy->width) + 1;
            row  = (i / copy->width) + 1;

            board_calculate(copy, other_color);
            if (board_can_play(board)) board_play(copy, other_color, col, row);
            board_calculate(copy, color);
            eval = minimax(color, copy, depth - 1, alpha, beta, true, 0, 0);
            board_free(&copy);

            if (eval < ret_eval) {
                ret_eval = eval;
                if (play_col) *play_col = col;
                if (play_row) *play_row = row;
            }

            beta = beta < eval ? beta : eval;
            if (beta <= alpha) break;
        }
    }

    return ret_eval;
}
