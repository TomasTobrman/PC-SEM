#ifndef _OTHELLO_BOARD_H_
#define _OTHELLO_BOARD_H_

#include "othello/bool.h"
#include "othello/player.h"

typedef struct {
    unsigned int width;
    unsigned int height;
    unsigned int size;
    player_color *data;
    unsigned int *moves;
} board_t;

board_t *board_create(unsigned int width, unsigned int height);
board_t *board_copy(board_t *original);
bool board_init(board_t *board, unsigned int col, unsigned int row);
void board_free(board_t **board);
void board_draw(board_t *board);
void board_calculate(board_t **board, player_color color);
bool board_play(board_t **board, player_color color, unsigned int col,
                unsigned int row);
bool board_can_play(board_t *board);
int board_count(board_t *board, player_color color);

#endif /* ifndef _OTHELLO_BOARD_H_ */
