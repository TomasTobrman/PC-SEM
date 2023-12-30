#ifndef _OTHELLO_BOARD_H_
#define _OTHELLO_BOARD_H_

#include "othello/bool.h"
#include "othello/player.h"

#define STONE_NONE  "·"
#define STONE_WHITE "◙"
#define STONE_BLACK "○"

typedef struct _board board_t;
board_t *board_create(unsigned int width, unsigned int height);
void board_init(board_t *board, unsigned int x, unsigned int y);
void board_free(board_t **board);
void board_draw(board_t *board);
void board_calculate(board_t *board, player_color color);
int board_play(board_t *board, player_color color, unsigned int x,
               unsigned int y);
bool board_can_play(board_t *board);
/* TODO: Player move */

#endif /* ifndef _OTHELLO_BOARD_H_ */
