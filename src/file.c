#include "othello/file.h"
#include "othello/history.h"
#include "othello/types.h"

#include <malloc/_malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool file_save(const settings_t *settings, char **history,
               unsigned int history_size) {
    unsigned int i = 0;
    if (!settings || !settings->file_out || !history) return false;

    /* save player black header */
    if (settings->player_black->type == PLAYER_TYPE_MINIMAX) {
        if (fprintf(settings->file_out, "[pb minimax,%s,%d]\n",
                    settings->player_black->name,
                    settings->player_black->depth) < 0)
            return false;
    } else {
        if (fprintf(settings->file_out, "[pb human,%s]\n",
                    settings->player_black->name) < 0)
            return false;
    }

    /* save player white header */
    if (settings->player_white->type == PLAYER_TYPE_MINIMAX) {
        if (fprintf(settings->file_out, "[pw minimax,%s,%d]\n",
                    settings->player_white->name,
                    settings->player_white->depth) < 0)
            return false;
    } else {
        if (fprintf(settings->file_out, "[pw human,%s]\n",
                    settings->player_white->name) < 0)
            return false;
    }

    /* save size header */
    if (fprintf(settings->file_out, "[size %d %d]\n", settings->board_rows,
                settings->board_cols) < 0)
        return false;

    /* save init header */
    if (fprintf(settings->file_out, "[init %d %d]\n", settings->init_row,
                settings->init_col) < 0)
        return false;

    /* save history */
    if (fprintf(settings->file_out, "[game]\n") < 0) return false;
    for (i = 0; i < history_size; ++i) {
        if (fprintf(settings->file_out, "%s", history[i]) < 0) return false;
    }

    return true;
}

/* TODO: check validity of the gamefile structure while processing */
char **file_load(settings_t *settings, unsigned int *history_size) {
    char **history    = NULL;
    char *line        = NULL;
    size_t line_len   = 0;
    bool reading_game = false;
    char *temp_str    = NULL;
    int temp_int      = 0;

    if (!settings || !settings->file_in) return NULL;

    history = (char **)malloc(sizeof(char *) * HISTORY_BUFFER_INCREMENT);
    if (!history) return NULL;
    *history_size = 0;

    while ((line = fgetln(settings->file_in, &line_len))) {
        /* TODO: read settings */

        /* initialize new `temp_str` of size `line_len` */
        temp_str = (char *)malloc(sizeof(char) * (line_len + 1));
        if (!temp_str) break;

        /* get just the current line from `line` */
        if (!(temp_str = strncpy(temp_str, line, line_len))) {
            free(temp_str);
            break;
        }

        /* terminate `temp_str` */
        temp_str[line_len] = '\0';

        /* start reading game */
        if (!strcmp(temp_str, "[game]\n")) {
            continue;
            reading_game = true;
        }

        if (reading_game) {
            /* add read moves to history */
            history = history_add(history, history_size, temp_str);
        } else {
            /* TODO: parse values */

            /* check for `[pb #]` tag */
            if (strstr(temp_str, "[pb ")) {
                printf("found -pb: %s", temp_str);
                continue;
            }

            /* check for `[pw #]` tag */
            if (strstr(temp_str, "[pw ")) {
                printf("found -pw: %s", temp_str);
                continue;
            }

            /* check for `[size # #]` tag */
            if (strstr(temp_str, "[size ")) {
                if (settings->size_set) continue;

                printf("found -size: %s", temp_str);
                temp_str = strtok(temp_str, " ");

                /* parse number of rows */
                temp_int = atoi(strtok(NULL, " "));
                if (temp_int > 1) settings->board_rows = temp_int;

                /* parse number of columns */
                temp_int = atoi(strtok(NULL, "]"));
                if (temp_int > 1) settings->board_cols = temp_int;

                continue;
            }

            /* check for `[init # #]` tag */
            if (strstr(temp_str, "[init ")) {
                if (settings->init_set) continue;

                printf("found -init: %s", temp_str);
                temp_str = strtok(temp_str, " ");

                temp_int = atoi(strtok(NULL, " "));
                if (temp_int > 0 && temp_int < settings->board_rows)
                    settings->init_row = temp_int;

                temp_int = atoi(strtok(NULL, "]"));
                if (temp_int > 0 && temp_int < settings->board_cols)
                    settings->init_col = temp_int;

                continue;
            }
        }

        free(temp_str);
        temp_str = NULL;
    }

    return history;
}
