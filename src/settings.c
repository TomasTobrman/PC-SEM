#include "othello/settings.h"
#include "othello/types.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int settings_set_default(settings_t *settings) {
    settings->player_white = (player_t *)malloc(sizeof(player_t));
    if (!settings->player_white) return 1;
    settings->player_white->type  = PLAYER_TYPE_HUMAN;
    settings->player_white->color = PLAYER_COLOR_WHITE;
    settings->player_white->name  = "Anna";
    settings->player_white->depth = 3;

    settings->player_black = (player_t *)malloc(sizeof(player_t));
    if (!settings->player_black) return 2;
    settings->player_black->type  = PLAYER_TYPE_HUMAN;
    settings->player_black->color = PLAYER_COLOR_BLACK;
    settings->player_black->name  = "Karel";
    settings->player_black->depth = 3;

    settings->board_cols  = 8;
    settings->board_rows  = 8;
    settings->init_col    = 4;
    settings->init_row    = 4;
    settings->file_out    = NULL;
    settings->file_in     = NULL;
    settings->should_exit = false;
    settings->next_move   = true;
    settings->interactive = true;

    return 0;
}

int settings_process_args(settings_t *settings, int argc, char *argv[]) {
    int i          = 0;
    int temp_int   = 0;
    char *temp_str = 0;

    for (i = 1; i < argc; ++i) {
        /* player black */
        if (!strcmp(argv[i], "-pb")) {
            if (i + 1 >= argc) {
                printf("Invalid player description!\n");
                return 1;
            }

            /* player type */
            temp_str = strtok(argv[++i], ",");
            if (!temp_str) {
                printf("Invalid player description!\n");
                return 1;
            }
            if (!strcmp(temp_str, "human")) {
                settings->player_black->type = PLAYER_TYPE_HUMAN;
            } else if (!strcmp(temp_str, "minimax")) {
                settings->player_black->type = PLAYER_TYPE_MINIMAX;
            } else {
                printf("Invalid player description!\n");
                return 1;
            }

            /* player name */
            temp_str = strtok(NULL, ",");
            if (temp_str) {
                if (temp_str[0] != '\0' &&
                    strcmp(temp_str, settings->player_white->name)) {
                    settings->player_black->name = temp_str;
                } else {
                    printf("Invalid player description!\n");
                    return 1;
                }
            }

            /* minimax tree depth */
            temp_str = strtok(NULL, ",");
            if (temp_str) {
                temp_int = atoi(temp_str);
                if (temp_str[0] == '\0' || temp_int < 0) {
                    printf("Invalid player description!\n");
                    return 1;
                }
                settings->player_black->depth = temp_int;
            }

            continue;
        }

        /* player white */
        if (!strcmp(argv[i], "-pw")) {
            if (i + 1 >= argc) {
                printf("Invalid player description!\n");
                return 1;
            }

            /* player type */
            temp_str = strtok(argv[++i], ",");
            if (!temp_str) {
                printf("Invalid player description!\n");
                return 1;
            }
            if (!strcmp(temp_str, "human")) {
                settings->player_white->type = PLAYER_TYPE_HUMAN;
            } else if (!strcmp(temp_str, "minimax")) {
                settings->player_white->type = PLAYER_TYPE_MINIMAX;
            } else {
                printf("Invalid player description!\n");
                return 1;
            }

            /* player name */
            temp_str = strtok(NULL, ",");
            if (temp_str) {
                if (temp_str[0] != '\0' &&
                    strcmp(temp_str, settings->player_black->name)) {
                    settings->player_white->name = temp_str;
                } else {
                    printf("Invalid player description!\n");
                    return 1;
                }
            }

            /* minimax tree depth */
            temp_str = strtok(NULL, ",");
            if (temp_str) {
                temp_int = atoi(temp_str);
                if (temp_str[0] == '\0' || temp_int < 0) {
                    printf("Invalid player description!\n");
                    return 1;
                }
                settings->player_white->depth = temp_int;
            }

            continue;
        }

        /* board size */
        if (!strcmp(argv[i], "-size")) {
            if (i + 2 >= argc) {
                printf("Invalid board size!\n");
                return 2;
            }

            /* board rows */
            temp_int = atoi(argv[i + 1]);
            if (temp_int < 2) {
                printf("Invalid board size!\n");
                return 2;
            }
            settings->board_rows = temp_int;

            /* board columns */
            temp_int = atoi(argv[i + 2]);
            if (temp_int < 2) {
                printf("Invalid board size!\n");
                return 2;
            }
            settings->board_cols = temp_int;

            i += 2;

            continue;
        }

        /* initial stones position */
        if (!strcmp(argv[i], "-init")) {
            if (i + 2 >= argc) {
                printf("Unable to place initial stones!\n");
                return 3;
            }

            /* top left stone row */
            temp_int = atoi(argv[i + 1]);
            if (temp_int >= settings->board_rows || temp_int < 1) {
                printf("Unable to place initial stones!\n");
                return 3;
            }
            settings->init_row = temp_int;

            /* top left stone column */
            temp_int = atoi(argv[i + 2]);
            if (temp_int >= settings->board_cols || temp_int < 1) {
                printf("Unable to place initial stones!\n");
                return 3;
            }
            settings->init_col = temp_int;

            i += 2;

            continue;
        }

        /* output gamefile */
        if (!strcmp(argv[i], "-o")) {
            if (i + 1 >= argc) {
                printf("Invalid output file destination!\n");
                return 4;
            }

            /* open output gamefile */
            settings->file_out = fopen(argv[++i], "w");
            if (!settings->file_in) {
                printf("Invalid output file destination!\n");
                return 4;
            }

            continue;
        }

        /* input gamefile */
        if (!strcmp(argv[i], "-i")) {
            if (i + 1 >= argc) {
                printf("Invalid input file!\n");
                return 5;
            }

            /* open input gamefile */
            settings->file_in = fopen(argv[++i], "r");
            if (!settings->file_in) {
                printf("Invalid input file!\n");
                return 5;
            }
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
