#ifndef _OTHELLO_SETTINGS_H_
#define _OTHELLO_SETTINGS_H_
#include "othello/bool.h"
#include "othello/player.h"
#include <stdio.h>
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
int settings_set_default(settings_t *settings);
int settings_process_args(settings_t *settings, int argc, char *argv[]);
void settings_free(settings_t **settings);
#endif /* ifndef _OTHELLO_SETTINGS_H_ */
