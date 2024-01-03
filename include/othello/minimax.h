#ifndef _OTHELLO_MINIMAX_H_
#define _OTHELLO_MINIMAX_H_

#include "othello/board.h"
#include "othello/player.h"

typedef struct _node {
    board_t *board;
    unsigned int weight;
    unsigned int play_col;
    unsigned int play_row;
    unsigned int n_children;
    struct _node **children;
} node_t;

void minimax_generate(int depth, node_t **root, player_color color);
void minimax_free(node_t **root);
void minimax_draw(node_t *root);

#endif /* ifndef _OTHELLO_MINIMAX_H_ */
