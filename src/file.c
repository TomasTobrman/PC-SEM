#include "othello/file.h"
#include "othello/history.h"
#include <stdio.h>

/**
 * Save game history into an output gamefile
 *
 * @param const settings_t * settings Application settings structure pointer
 * @param const history_t * history Game history with played commands
 * @return bool Returns false when an error occures, else returns true
 */
bool file_save(const settings_t *settings, const history_t *history) {
    unsigned int i = 0;
    if (!settings || !settings->file_out || !history || !history->commands)
        return false;

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
    for (i = 0; i < history->size; ++i) {
        if (fprintf(settings->file_out, "%s", history->commands[i]) < 0)
            return false;
    }

    return true;
}

/**
 * Parse player information from a line
 *
 * @param player_t * player Player structure into which to save parsed data
 * @param char * line Input line string to parse
 * @return bool Returns true on success, else returns false
 */
static bool parse_player(player_t *player, char *line) {
    /* get the data part */
    line = strtok(line, " ");
    line = strtok(NULL, "]");

    if (!line) return false;

    /* player type */
    line = strtok(line, ",");
    if (line && !player->type_set) {
        if (!strcmp(line, "minimax")) {
            player->type     = PLAYER_TYPE_MINIMAX;
            player->type_str = "minimax";
        } else if (!strcmp(line, "human")) {
            player->type     = PLAYER_TYPE_HUMAN;
            player->type_str = "human";
        }
    }

    /* player name */
    line = strtok(NULL, ",");
    if (line && !player->name_set) strcpy(player->name, line);

    /* player depth */
    line = strtok(NULL, ",");
    if (line && !player->depth_set) player->depth = atoi(line);

    return true;
}

/**
 * Parse a line from the input gamefile
 *
 * @param settings_t * settings Settings structure to save parsed data into
 * @param char * line Input line string to parse
 * @return bool Returns true on success, else returns false
 */
static bool parse_line(settings_t *settings, char *line) {
    int temp_int = 0;

    /* check for `[pb #]` tag */
    if (strstr(line, "[pb ")) {
        return parse_player(settings->player_black, line);
    }

    /* check for `[pw #]` tag */
    if (strstr(line, "[pw ")) {
        return parse_player(settings->player_white, line);
    }

    /* check for `[size # #]` tag */
    if (strstr(line, "[size ")) {
        if (settings->size_set) return true;

        line = strtok(line, " ");

        /* parse number of rows */
        temp_int = atoi(strtok(NULL, " "));
        if (temp_int > 1) settings->board_rows = temp_int;
        else return false;

        /* parse number of columns */
        temp_int = atoi(strtok(NULL, "]"));
        if (temp_int > 1) settings->board_cols = temp_int;
        else return false;

        return true;
    }

    /* check for `[init # #]` tag */
    if (strstr(line, "[init ")) {
        if (settings->init_set) return true;

        line = strtok(line, " ");

        temp_int = atoi(strtok(NULL, " "));
        if (temp_int > 0 && temp_int < settings->board_rows)
            settings->init_row = temp_int;
        else return false;

        temp_int = atoi(strtok(NULL, "]"));
        if (temp_int > 0 && temp_int < settings->board_cols)
            settings->init_col = temp_int;
        else return false;

        return true;
    }

    return false;
}

/**
 * Load and parse an input gamefile
 *
 * @param settings_t * settings Settings structure to save parsed data into
 * @return History filled with moves from the input gamefile or NULL on error
 */
history_t *file_load(settings_t *settings) {
    history_t *history = NULL;
    char *input        = NULL;
    char *line         = NULL;
    size_t line_len    = 0;
    bool read_game     = false;

    if (!settings || !settings->file_in) return NULL;

    history = history_create();
    if (!history) return NULL;

    while ((input = fgetln(settings->file_in, &line_len))) {
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

        if (read_game) {
            /* read the game history from input gamefile */
            history_add(history, line);
        } else {
            /* try to parse the current input line */
            if (!parse_line(settings, line)) return NULL;
        }

        free(line);
        line = NULL;
    }

    return history;
}
