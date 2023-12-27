#ifndef _OTHELLO_BOARD_H_
#define _OTHELLO_BOARD_H_

#define PIECE_WHITE " ◙ "
#define PIECE_BLACK " ○ "
#define PIECE_BLANK " · "

#include <bits/stdint-uintn.h>

typedef struct _board board_t;
typedef enum { PLAYER_NONE, PLAYER_WHITE, PLAYER_BLACK } player_t;

board_t *board_create(uint8_t width, uint8_t height);
void board_init(board_t *board, uint8_t row, uint8_t col);
void board_free(board_t **board);
void board_draw(board_t *board);
uint8_t board_play(board_t *board, player_t player, uint8_t row, uint8_t col);

#endif /* ifndef _OTHELLO_BOARD_H_ */
