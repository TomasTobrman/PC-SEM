#ifndef _OTHELLO_TYPES_H_
#define _OTHELLO_TYPES_H_

#include <ctype.h>
#include <i386/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ==== GLOBAL DEFINES ====================================================== */

#define INPUT_SIZE        0xFF
#define HISTORY_INCREMENT 32

#define STONE_NONE  "·"
#define STONE_BLACK "○"
#define STONE_WHITE "◙"

#define CMD_STR_SAVE "save"
#define CMD_STR_QUIT "quit"
#define CMD_STR_PASS "pass"

/* ==== ENUMS =============================================================== */

/* bool type implementation */
typedef enum { false = 0, true = 1 } bool;

/* What is the player played by */
typedef enum {
    /* Human player */
    PLAYER_TYPE_HUMAN = 0,
    /* Minimax algorithm */
    PLAYER_TYPE_MINIMAX = 1
} player_type;

/* Color of the player stone */
typedef enum {
    /* Stone has no color or cell is blank */
    PLAYER_COLOR_NONE = 0,
    /* Black stone color */
    PLAYER_COLOR_BLACK = 1,
    /* White stone color */
    PLAYER_COLOR_WHITE = 2
} player_color;

/* ==== STRUCTS ============================================================= */

/* Board structure holding the game data and available moves */
typedef struct {
    /* Number of board columns */
    unsigned int width;
    /* Number of board rows */
    unsigned int height;
    /* Size of data and moves arrays */
    unsigned int size;
    /* Player stones positions */
    player_color *data;
    /* Available moves */
    unsigned int *moves;
} board_t;

/* Command history */
typedef struct {
    /* Array of commands */
    char **commands;
    /* Size of the commands array */
    size_t size;
} history_t;

/* Player info */
typedef struct {
    /* Player color */
    player_color color;
    /* If the player is played by a human or minimax */
    player_type type;
    /* String name of the player type */
    char *type_str;
    /* If the type has been set from the default value */
    bool type_set;
    /* Name of the player */
    char name[INPUT_SIZE];
    /* If the name has been set from the default value */
    bool name_set;
    /* Max depth of the minimax algorithm for his player */
    int depth;
    /* If the depth has been set from the default value */
    bool depth_set;
} player_t;

/* Application settings */
typedef struct {
    /* Black Player */
    player_t *player_black;
    /* White Player */
    player_t *player_white;
    /* Number of board columns */
    int board_cols;
    /* Number of board rows */
    int board_rows;
    /* If the board size has been set from the default value */
    bool size_set;
    /* Column of the initial top left stone */
    int init_col;
    /* Row of the initial top left stone */
    int init_row;
    /* If the initial position has been set from the default value */
    bool init_set;
    /* Output gamefile pointer */
    FILE *file_out;
    /* Input gamefile pointer */
    FILE *file_in;
    /* If the application should exit - ends the game loop */
    bool should_exit;
    /* If the players should be switched and a next move should be played */
    bool next_move;
} settings_t;

#endif /* ifndef _OTHELLO_TYPES_H_ */
