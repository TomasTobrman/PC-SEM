#ifndef _OTHELLO_BOARD_H_
#define _OTHELLO_BOARD_H_
#include "othello/types.h"
board_t *board_create(unsigned int width, unsigned int height);
board_t *board_copy(const board_t *original);
bool board_init(board_t *board, unsigned int col, unsigned int row);
void board_free(board_t **board);
void board_draw(const board_t *board);
void board_calculate(board_t **board, player_color color);
bool board_play(board_t **board, player_color color, unsigned int col,
                unsigned int row);
bool board_can_play(board_t *board);
int board_count(const board_t *board, player_color color);
int board_eval(const board_t *board);
#endif /* ifndef _OTHELLO_BOARD_H_ */
