#ifndef _OTHELLO_MINIMAX_H_
#define _OTHELLO_MINIMAX_H_

#include "othello/board.h"

typedef struct _leaf {
    board_t *board;
    struct _leaf *next;
} leaf_t;

typedef struct _tree {
} tree_t;

#endif /* ifndef _OTHELLO_MINIMAX_H_ */
