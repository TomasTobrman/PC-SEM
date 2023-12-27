#ifndef _OTHELLO_PLAYER_H_
#define _OTHELLO_PLAYER_H_
typedef enum { PLAYER_TYPE_HUMAN, PLAYER_TYPE_MINIMAX } player_type;
typedef enum {
  PLAYER_COLOR_NONE,
  PLAYER_COLOR_WHITE,
  PLAYER_COLOR_BLACK
} player_color;
typedef struct {
  player_type type;
  player_color color;
  char *name;
} player_t;
#endif /* ifndef _OTHELLO_PLAYER_H_ */
