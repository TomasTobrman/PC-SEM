#ifndef _OTHELLO_MINIMAX_H_
#define _OTHELLO_MINIMAX_H_

#include "othello/types.h"

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
            bool maximizing, unsigned int *play_col, unsigned int *play_row);

#endif /* ifndef _OTHELLO_MINIMAX_H_ */
