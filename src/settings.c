#include "othello/settings.h"
#include "othello/types.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int settings_set_default(settings_t *settings) {
    settings->player_black = (player_t *)malloc(sizeof(player_t));
    if (!settings->player_black) return 1;
    settings->player_black->type      = PLAYER_TYPE_HUMAN;
    settings->player_black->color     = PLAYER_COLOR_BLACK;
    settings->player_black->name      = "Karel";
    settings->player_black->depth     = 3;
    settings->player_black->name_set  = false;
    settings->player_black->type_set  = false;
    settings->player_black->depth_set = false;

    settings->player_white = (player_t *)malloc(sizeof(player_t));
    if (!settings->player_white) return 2;
    settings->player_white->type      = PLAYER_TYPE_HUMAN;
    settings->player_white->color     = PLAYER_COLOR_WHITE;
    settings->player_white->name      = "Anna";
    settings->player_white->depth     = 3;
    settings->player_white->name_set  = false;
    settings->player_white->type_set  = false;
    settings->player_white->depth_set = false;

    settings->board_cols  = 8;
    settings->board_rows  = 8;
    settings->init_col    = 4;
    settings->init_row    = 4;
    settings->file_out    = NULL;
    settings->file_in     = NULL;
    settings->should_exit = false;
    settings->next_move   = true;
    settings->interactive = true;

    settings->size_set = false;
    settings->init_set = false;

    return 0;
}

static int read_player(player_t *player, const char *other_name, int argc,
                       char **argv, int *i) {
    char *temp_str = NULL;
    int temp_int   = 0;

    if (*i + 1 >= argc) return false;

    /* player type */
    temp_str = strtok(argv[++*i], ",");
    if (!temp_str) return false;

    if (!strcmp(temp_str, "human")) {
        player->type     = PLAYER_TYPE_HUMAN;
        player->type_set = true;
    } else if (!strcmp(temp_str, "minimax")) {
        player->type     = PLAYER_TYPE_MINIMAX;
        player->type_set = true;
    } else return false;

    /* player name */
    temp_str = strtok(NULL, ",");
    if (temp_str) {
        if (temp_str[0] != '\0' && strcmp(temp_str, other_name)) {
            player->name     = temp_str;
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

static bool read_file(const char *method, FILE **file, int argc, char **argv,
                      int *i) {
    if (*i + 1 >= argc) return false;

    /* open output gamefile */
    *file = fopen(argv[++*i], method);
    if (!*file) return false;

    return true;
}

int settings_process_args(settings_t *settings, int argc, char *argv[]) {
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
            settings->interactive = false;
            continue;
        }
    }

    return 0;
}

void settings_free(settings_t **settings) {
    if (!settings || !*settings) return;
    if ((*settings)->file_out) fclose((*settings)->file_out);
    if ((*settings)->file_in) fclose((*settings)->file_in);
    if ((*settings)->player_white) free((*settings)->player_white);
    if ((*settings)->player_black) free((*settings)->player_black);
    free(*settings);
    settings = NULL;
}
