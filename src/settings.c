#include "othello/settings.h"
#include "othello/types.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Create new application settings
 *
 * @return settings_t * Allocated structure pointer
 */
settings_t *settings_create() {
    /* allocate the settings structure pointer */
    settings_t *settings = (settings_t *)malloc(sizeof(settings_t));
    if (!settings) return NULL;

    /* allocate the black player structure pointer */
    settings->player_black = (player_t *)malloc(sizeof(player_t));
    if (!settings->player_black) {
        settings_free(&settings);
        return NULL;
    }

    /* allocate the white player structure pointer */
    settings->player_white = (player_t *)malloc(sizeof(player_t));
    if (!settings->player_white) {
        settings_free(&settings);
        return NULL;
    }

    /* set players' colors */
    settings->player_black->color = PLAYER_COLOR_BLACK;
    settings->player_white->color = PLAYER_COLOR_WHITE;

    /* set players' type */
    settings->player_black->type     = PLAYER_TYPE_HUMAN;
    settings->player_white->type     = PLAYER_TYPE_HUMAN;
    settings->player_black->type_str = "human";
    settings->player_white->type_str = "human";
    settings->player_black->type_set = false;
    settings->player_white->type_set = false;

    /* set black player's name */
    strcpy(settings->player_black->name, "Karel");
    settings->player_black->name_set = false;

    /* set white player's name */
    strcpy(settings->player_white->name, "Anna");
    settings->player_white->name_set = false;

    /* set players' minimax search depth */
    settings->player_black->depth     = 3;
    settings->player_white->depth     = 3;
    settings->player_black->depth_set = false;
    settings->player_white->depth_set = false;

    /* set board size */
    settings->board_cols = 8;
    settings->board_rows = 8;
    settings->size_set   = false;

    /* set initial stone placement */
    settings->init_col = 4;
    settings->init_row = 4;
    settings->init_set = false;

    /* set input and output gamefiles to NULL */
    settings->file_in  = NULL;
    settings->file_out = NULL;

    /* application should not exit yet */
    settings->should_exit = false;

    /* process the next move */
    settings->next_move = true;

    return settings;
}

/**
 * Read the player informations from command-line arguments
 *
 * @param player_t * player Player structure pointer to save read data into
 * @param const char * other_name Name of the other player for duplicity check
 * @param int argc Number of command-line arguments
 * @param char ** argv Values of command-line arguments
 * @param int * i Current index in the command-line arguments array
 * @return bool Returns true if the player was successfully read, else false
 */
static bool read_player(player_t *player, const char *other_name, int argc,
                        char **argv, int *i) {
    char *temp_str = NULL;
    int temp_int   = 0;

    if (*i + 1 >= argc) return false;

    /* player type */
    temp_str = strtok(argv[++*i], ",");
    if (!temp_str) return false;

    if (!strcmp(temp_str, "human")) {
        player->type     = PLAYER_TYPE_HUMAN;
        player->type_str = "human";
        player->type_set = true;
    } else if (!strcmp(temp_str, "minimax")) {
        player->type     = PLAYER_TYPE_MINIMAX;
        player->type_str = "minimax";
        player->type_set = true;
    } else return false;

    /* player name */
    temp_str = strtok(NULL, ",");
    if (temp_str) {
        if (temp_str[0] != '\0' && strcmp(temp_str, other_name)) {
            strcpy(player->name, temp_str);
            player->name_set = true;
        } else return false;
    }

    /* minimax tree depth */
    temp_str = strtok(NULL, ",");
    if (temp_str) {
        temp_int = atoi(temp_str);
        if (temp_str[0] == '\0' || temp_int < 0) return false;
        player->depth     = temp_int;
        player->depth_set = true;
    }

    return true;
}

/**
 * Read the board size from command-line arguments
 *
 * @param settigns_t * settings Settings structure to save read data into
 * @param int argc Number of command-line arguments
 * @param char ** argv Values of command-line arguments
 * @param int * i Current index in the command-line arguments array
 * @return bool Returns true if the player was successfully read, else false
 */
static bool read_size(settings_t *settings, int argc, char **argv, int *i) {
    int temp_int = 0;

    if (*i + 2 >= argc) return false;

    /* board rows */
    temp_int = atoi(argv[*i + 1]);
    if (temp_int < 2) return false;
    settings->board_rows = temp_int;

    /* board columns */
    temp_int = atoi(argv[*i + 2]);
    if (temp_int < 2) return false;
    settings->board_cols = temp_int;

    *i += 2;
    return true;
}

/**
 * Read the initial stone placement from command-line arguments
 *
 * @param settigns_t * settings Settings structure to save read data into
 * @param int argc Number of command-line arguments
 * @param char ** argv Values of command-line arguments
 * @param int * i Current index in the command-line arguments array
 * @return bool Returns true if the player was successfully read, else false
 */
static bool read_init(settings_t *settings, int argc, char **argv, int *i) {
    int temp_int = 0;

    if (*i + 2 >= argc) return false;

    /* init row */
    temp_int = atoi(argv[*i + 1]);
    if (temp_int < 1 || temp_int >= settings->board_rows) return false;
    settings->init_row = temp_int;

    /* init column */
    temp_int = atoi(argv[*i + 2]);
    if (temp_int < 1 || temp_int >= settings->board_cols) return false;
    settings->init_col = temp_int;

    *i += 2;
    return true;
}

/**
 * Read a file path from command-line arguments
 *
 * @param const char * method File open method
 * @param FILE ** file Address of a FILE pointer to save the opened file into
 * @param int argc Number of command-line arguments
 * @param char ** argv Values of command-line arguments
 * @param int * i Current index in the command-line arguments array
 * @return bool Returns true if the player was successfully read, else false
 */
static bool read_file(const char *method, FILE **file, int argc, char **argv,
                      int *i) {
    if (*i + 1 >= argc) return false;

    /* open output gamefile */
    *file = fopen(argv[++*i], method);
    if (!*file) return false;

    return true;
}

/**
 * Process and assing values from command-line arguments
 *
 * @param settigns_t * settings Settings structure to save read data into
 * @param int argc Number of command-line arguments
 * @param char ** argv Values of command-line arguments
 * @return int Returns 0 when successfully parsed, else returns error code
 */
int settings_process_args(settings_t *settings, int argc, char **argv) {
    int i = 0;

    if (!settings || !argv) return 0;

    for (i = 1; i < argc; ++i) {
        /* player black */
        if (!strcmp(argv[i], "-pb")) {
            if (!read_player(settings->player_black,
                             settings->player_white->name, argc, argv, &i))
                return 1;
            continue;
        }

        /* player white */
        if (!strcmp(argv[i], "-pw")) {
            if (!read_player(settings->player_white,
                             settings->player_black->name, argc, argv, &i))
                return 1;
            continue;
        }

        /* board size */
        if (!strcmp(argv[i], "-size")) {
            if (!read_size(settings, argc, argv, &i)) return 2;
            settings->size_set = true;
            continue;
        }

        /* initial stones position */
        if (!strcmp(argv[i], "-init")) {
            if (!read_init(settings, argc, argv, &i)) return 3;
            settings->init_set = true;
            continue;
        }

        /* output gamefile */
        if (!strcmp(argv[i], "-o")) {
            if (!read_file("w", &settings->file_out, argc, argv, &i)) return 4;
            continue;
        }

        /* input gamefile */
        if (!strcmp(argv[i], "-i")) {
            if (!read_file("r", &settings->file_in, argc, argv, &i)) return 5;
            continue;
        }
    }

    return 0;
}

/**
 * Free the settigns_t structure
 *
 * @param settings_t ** settings Address of the settings_t structure pointer
 */
void settings_free(settings_t **settings) {
    if (!settings || !*settings) return;
    if ((*settings)->file_out) fclose((*settings)->file_out);
    if ((*settings)->file_in) fclose((*settings)->file_in);
    if ((*settings)->player_white) free((*settings)->player_white);
    if ((*settings)->player_black) free((*settings)->player_black);
    free(*settings);
    settings = NULL;
}
