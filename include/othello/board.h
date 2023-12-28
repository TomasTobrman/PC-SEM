#ifndef _OTHELLO_BOARD_H_
#define _OTHELLO_BOARD_H_

#include "othello/player.h"
#include <stdint.h>

#define STONE_NONE  " · "
#define STONE_WHITE " ◙ "
#define STONE_BLACK " ○ "

typedef struct _board board_t;
board_t *board_create(uint8_t width, uint8_t height);
void board_init(board_t *board, uint8_t row, uint8_t col);
void board_free(board_t **board);
void board_draw(board_t *board);
void board_draw_moves(board_t *board);
void board_calculate(board_t *board, player_color color);
/* TODO: Player move */

#endif /* ifndef _OTHELLO_BOARD_H_ */
