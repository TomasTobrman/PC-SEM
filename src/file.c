#include "othello/file.h"
#include "othello/history.h"
#include "othello/types.h"

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
        if (fprintf(settings->file_out, "[pb minimax,%s]\n",
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
        if (fprintf(settings->file_out, "[pw minimax,%s]\n",
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

char **file_load(settings_t *settings, unsigned int *history_size) {
    char **history    = NULL;
    char *line        = NULL;
    size_t line_len   = 0;
    bool reading_game = false;

    if (!settings || !settings->file_in) return NULL;

    history = (char **)malloc(sizeof(char *) * HISTORY_BUFFER_INCREMENT);
    if (!history) return NULL;
    history_size = 0;

    while ((line = fgetln(settings->file_in, &line_len))) {
        /* TODO: read settings */

        /* start reading game */
        if (!strcmp(line, "[game]")) {
            reading_game = true;
            continue;
        }

        /* add read moves to history */
        /* TODO: check moves before adding */
        if (reading_game) history = history_add(history, history_size, line);
    }

    return history;
}
