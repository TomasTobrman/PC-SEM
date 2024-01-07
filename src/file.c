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

static void parse_player(player_t *player, char *line) {
    /* get the data part */
    line = strtok(line, " ");
    line = strtok(NULL, "]");

    /* player type */
    line = strtok(line, ",");
    if (line && !player->type_set)
        player->type =
            !strcmp(line, "minimax") ? PLAYER_TYPE_MINIMAX : PLAYER_TYPE_HUMAN;

    /* player name */
    line = strtok(NULL, ",");
    if (line && !player->name_set) strcpy(player->name, line);

    /* player depth */
    line = strtok(NULL, ",");
    if (line && !player->depth_set) player->depth = atoi(line);
}

static void parse_file(settings_t *settings, char *line) {
    int temp_int = 0;

    /* check for `[pb #]` tag */
    if (strstr(line, "[pb ")) {
        parse_player(settings->player_black, line);
        return;
    }

    /* check for `[pw #]` tag */
    if (strstr(line, "[pw ")) {
        parse_player(settings->player_white, line);
        return;
    }

    /* check for `[size # #]` tag */
    if (strstr(line, "[size ")) {
        if (settings->size_set) return;

        line = strtok(line, " ");

        /* parse number of rows */
        temp_int = atoi(strtok(NULL, " "));
        if (temp_int > 1) settings->board_rows = temp_int;

        /* parse number of columns */
        temp_int = atoi(strtok(NULL, "]"));
        if (temp_int > 1) settings->board_cols = temp_int;

        return;
    }

    /* check for `[init # #]` tag */
    if (strstr(line, "[init ")) {
        if (settings->init_set) return;

        line = strtok(line, " ");

        temp_int = atoi(strtok(NULL, " "));
        if (temp_int > 0 && temp_int < settings->board_rows)
            settings->init_row = temp_int;

        temp_int = atoi(strtok(NULL, "]"));
        if (temp_int > 0 && temp_int < settings->board_cols)
            settings->init_col = temp_int;

        return;
    }
}

/* TODO: check validity of the gamefile structure while processing */
char **file_load(settings_t *settings, unsigned int *hist_len) {
    char **history  = NULL;
    char *input     = NULL;
    char *line      = NULL;
    size_t line_len = 0;
    bool read_game  = false;

    if (!settings || !settings->file_in) return NULL;

    history = (char **)malloc(sizeof(char *) * HISTORY_BUFFER_INCREMENT);
    if (!history) return NULL;
    *hist_len = 0;

    while ((input = fgetln(settings->file_in, &line_len))) {
        /* TODO: read settings */

        /* initialize new `temp_str` of size `line_len` */
        line = (char *)malloc(sizeof(char) * (line_len + 1));
        if (!line) break;

        /* get just the current line from `line` */
        if (!(line = strncpy(line, input, line_len))) {
            free(line);
            break;
        }

        /* terminate `temp_str` */
        line[line_len] = '\0';

        /* start reading game */
        if (!strcmp(line, "[game]\n")) {
            read_game = true;
            continue;
        }

        if (read_game) history_add(history, hist_len, line);
        else parse_file(settings, line);

        free(line);
        line = NULL;
    }

    return history;
}
