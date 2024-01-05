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
    char **history       = NULL;
    char *line           = NULL;
    size_t line_len      = 0;
    bool reading_history = false;
    char *temp_str       = NULL;
    int temp_int         = 0;

    if (!settings || !settings->file_in) return NULL;

    history = (char **)malloc(sizeof(char *) * HISTORY_BUFFER_INCREMENT);
    if (!history) return NULL;
    history_size = 0;

    while ((line = fgetln(settings->file_in, &line_len))) {
        if ((temp_str = strstr(line, "[pb "))) {
            strtok(temp_str, " ");

            /* player type */
            temp_str = strtok(NULL, ",");
            if (!temp_str) {
                printf("Invalid player description!\n");
                return NULL;
            }
            if (!strcmp(temp_str, "human")) {
                settings->player_black->type = PLAYER_TYPE_HUMAN;
            } else if (!strcmp(temp_str, "minimax")) {
                settings->player_black->type = PLAYER_TYPE_MINIMAX;
            } else {
                printf("Invalid player description!\n");
                return NULL;
            }

            /* player name */
            temp_str = strtok(NULL, ",");
            if (temp_str) {
                if (temp_str[0] != '\0' &&
                    strcmp(temp_str, settings->player_white->name)) {
                    settings->player_black->name = temp_str;
                } else {
                    printf("Invalid player description!\n");
                    return NULL;
                }
            }

            /* minimax tree depth */
            temp_str = strtok(NULL, ",");
            if (temp_str) {
                temp_int = atoi(temp_str);
                if (temp_str[0] == '\0' || temp_int < 0) {
                    printf("Invalid player description!\n");
                    return NULL;
                }
                settings->player_black->depth = temp_int;
            }
        } else if ((temp_str = strstr(line, "[pw "))) {
            strtok(temp_str, " ");

            /* player type */
            temp_str = strtok(NULL, ",");
            if (!temp_str) {
                printf("Invalid player description!\n");
                return NULL;
            }
            if (!strcmp(temp_str, "human")) {
                settings->player_white->type = PLAYER_TYPE_HUMAN;
            } else if (!strcmp(temp_str, "minimax")) {
                settings->player_white->type = PLAYER_TYPE_MINIMAX;
            } else {
                printf("Invalid player description!\n");
                return NULL;
            }

            /* player name */
            temp_str = strtok(NULL, ",");
            if (temp_str) {
                if (temp_str[0] != '\0' &&
                    strcmp(temp_str, settings->player_black->name)) {
                    settings->player_white->name = temp_str;
                } else {
                    printf("Invalid player description!\n");
                    return NULL;
                }
            }

            /* minimax tree depth */
            temp_str = strtok(NULL, ",");
            if (temp_str) {
                temp_int = atoi(temp_str);
                if (temp_str[0] == '\0' || temp_int < 0) {
                    printf("Invalid player description!\n");
                    return NULL;
                }
                settings->player_white->depth = temp_int;
            }
        } else if ((temp_str = strstr(line, "[size "))) {
            strtok(temp_str, " ");

            /* board rows */
            temp_int = atoi(strtok(NULL, " "));
            if (temp_int < 2) {
                printf("%d", temp_int);
                printf("Invalid board size!\n");
                return NULL;
            }
            settings->board_rows = temp_int;

            /* board columns */
            temp_int = atoi(strtok(NULL, " "));
            if (temp_int < 2) {
                printf("%d", temp_int);
                printf("Invalid board size!\n");
                return NULL;
            }
            settings->board_cols = temp_int;
        } else if ((temp_str = strstr(line, "[init "))) {
            strtok(temp_str, " ");
            printf("%s\n", temp_str);

            /* top left stone row */
            temp_int = atoi(strtok(NULL, " "));
            if (temp_int >= settings->board_rows || temp_int < 1) {
                printf("Unable to place initial stones!\n");
                return NULL;
            }
            settings->init_row = temp_int;

            /* top left stone column */
            temp_int = atoi(strtok(NULL, " "));
            if (temp_int >= settings->board_cols || temp_int < 1) {
                printf("Unable to place initial stones!\n");
                return NULL;
            }
            settings->init_col = temp_int;
        } else if (!strcmp(line, "[game]")) {
            /* start reading game */
            reading_history = true;
        } else {
            return NULL;
        }

        if (reading_history) history_add(history, history_size, line);
    }

    return history;
}
