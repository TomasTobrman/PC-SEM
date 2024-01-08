#ifndef _OTHELLO_MINIMAX_H_
#define _OTHELLO_MINIMAX_H_
#include "othello/types.h"
int minimax(player_color color, board_t *board, int depth, int alpha, int beta,
            bool maximizing, unsigned int *play_col, unsigned int *play_row);
#endif /* ifndef _OTHELLO_MINIMAX_H_ */
