#ifndef _OTHELLO_PLAYER_H_
#define _OTHELLO_PLAYER_H_
typedef struct {
  enum { PLAYER_TYPE_HUMAN, PLAYER_TYPE_MINIMAX } type;
  enum { PLAYER_COLOR_WHITE, PLAYER_COLOR_BLACK } color;
  char *name;
} player_t;
#endif /* ifndef _OTHELLO_PLAYER_H_ */
