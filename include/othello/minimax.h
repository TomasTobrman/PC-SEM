#ifndef _OTHELLO_MINIMAX_H_
#define _OTHELLO_MINIMAX_H_
#include "othello/types.h"
void minimax_generate(int depth, node_t **root, player_color color,
                      bool opponent);
void minimax_free(node_t **root);
void minimax_draw(node_t *root);
#endif /* ifndef _OTHELLO_MINIMAX_H_ */
