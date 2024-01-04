#ifndef _OTHELLO_MINIMAX_H_
#define _OTHELLO_MINIMAX_H_
#include "othello/types.h"
void minimax_generate(int depth, node_t **root, player_color color,
                      bool opponent);
void minimax_free(node_t **root);
void minimax_draw(node_t *root);
int minimax(player_color color, const board_t *board, int depth, int alpha,
            int beta, bool maximizing, unsigned int *play_col,
            unsigned int *play_row);
#endif /* ifndef _OTHELLO_MINIMAX_H_ */
