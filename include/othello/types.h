#ifndef _OTHELLO_TYPES_H_
#define _OTHELLO_TYPES_H_
#include <stdio.h>
/* ==== ENUMS =============================================================== */
typedef enum { false, true } bool;
typedef enum { CMD_NONE, CMD_QUIT, CMD_SAVE, CMD_PASS, CMD_PLAY } command_code;
typedef enum { PLAYER_TYPE_HUMAN, PLAYER_TYPE_MINIMAX } player_type;
typedef enum {
    PLAYER_COLOR_NONE,
    PLAYER_COLOR_WHITE,
    PLAYER_COLOR_BLACK
} player_color;
/* ==== STRUCTS ============================================================= */
typedef struct {
    unsigned int width;
    unsigned int height;
    unsigned int size;
    player_color *data;
    unsigned int *moves;
} board_t;
typedef struct {
    player_type type;
    player_color color;
    char *name;
    int depth;
} player_t;
typedef struct {
    player_t *player_white;
    player_t *player_black;
    int board_cols;
    int board_rows;
    int init_col;
    int init_row;
    FILE *file_out;
    FILE *file_in;
    bool should_exit;
    bool next_move;
    bool interactive;
} settings_t;
#endif /* ifndef _OTHELLO_TYPES_H_ */
