#include "othello/othello.h"
#include <stdio.h>

/**
 * Create everything needed for a game of Othello
 *
 * @param int argc Number of command-line arguments
 * @param char ** argv Values of command-line arguments
 * @param settings_t ** settings Address of settings structure to return into
 * @param board_t ** board Address of board structure to return into
 * @param history_t ** history Address of history structure to return into
 * @return int Returns exit status when error occures, else returns 0
 */
int create_game(int argc, char **argv, settings_t **settings, board_t **board,
                history_t **history) {
    int args_status       = 0;
    const char *errors[6] = {
        "Invalid player description!\n",
        "Invalid board size!\n",
        "Unable to place initial stones!\n",
        "Invalid output file destination!\n",
        "Invalid input file!\n",
        "something went wrong...\n",
    };

    /* set default settings */
    *settings = settings_create();
    if (!*settings) {
        printf("%s", errors[5]);
        return -1;
    }

    /* process command-line arguments */
    if ((args_status = settings_process_args(*settings, argc, argv))) {
        settings_free(settings);
        printf("%s", errors[args_status - 1]);
        return args_status;
    }

    /* load in predefined moves */
    *history = file_load(*settings);
    if ((*settings)->file_in && !*history) {
        printf("%s", errors[4]);
        return 5;
    }

    /* create a new board */
    *board = board_create((*settings)->board_cols, (*settings)->board_rows);
    if (!board) {
        if (history) history_free(history);
        settings_free(settings);
        printf("%s", errors[5]);
        return -1;
    }

    /* place initial stones */
    if (!board_init(*board, (*settings)->init_col, (*settings)->init_row)) {
        if (history) history_free(history);
        board_free(board);
        settings_free(settings);
        printf("%s", errors[2]);
        return 3;
    }

    return 0;
}

/**
 * Change the current player to the next one and recalculate the board
 *
 * @param settings_t * settings Application settings structure pointer
 * @param board_t * board Board structure pointer to recalculate
 * @param player_t ** cur_player Address of current player variable to change
 * @return bool Returns true if the game should end, else returns false
 */
bool next_move(settings_t *settings, board_t *board, player_t **cur_player) {
    if (!settings || !board || !cur_player) return true;

    /* is game over */
    if (is_game_over(board)) {
        settings->should_exit = true;
        return true;
    }

    /* switch players */
    *cur_player = *cur_player == settings->player_black
                    ? settings->player_white
                    : settings->player_black;

    /* calculate possible moves */
    board_calculate(board, (*cur_player)->color);

    /* redraw the board */
    board_draw(board);

    /* reset state */
    settings->next_move = false;
    return false;
}

/**
 * Process a command and respond to it
 *
 * @param board_t * board Board structure pointer on which game is played
 * @param player_t * cur_player Current player to which assign the command
 * @param settings_t * settings Application settings structure pointer
 * @param history_t * history Command history to save the command into
 * @param const char * command String command to process
 * @return bool Returns true on successful command process, else returns false
 */
bool play_command(board_t *board, player_t *cur_player, settings_t *settings,
                  history_t *history, const char *command) {
    int i                 = 0;
    char *temp_str        = 0;
    char *tok_str         = 0;
    unsigned int play_col = 0;
    unsigned int play_row = 0;

    if (!command || !board || !cur_player || !settings) return false;

    /* copy the command string so it can be altered */
    temp_str = (char *)malloc(strlen(command) + 1);
    if (!temp_str) return false;
    strcpy(temp_str, command);

    /* remove trailing new-line character */
    temp_str[strcspn(temp_str, "\r\n")] = 0;

    /* input to lowercase */
    for (i = 0; temp_str[i]; i++) temp_str[i] = tolower(temp_str[i]);

    /* quit command given */
    if (!strcmp(temp_str, CMD_STR_QUIT)) {
        /* free the temp_str as there is no other use for it */
        free(temp_str);

        /* application should exit */
        settings->should_exit = true;
        return true;
    }

    /* pass command given */
    if (!strcmp(temp_str, CMD_STR_PASS)) {
        /* free the temp_str as there is no other use for it */
        free(temp_str);

        /* is this a valid move */
        if (board_can_play(board)) {
            printf("Invalid move!\n");
            return true;
        }

        /* process the next move after */
        settings->next_move = true;

        /* add this move to the history */
        if (history) history_add(history, command);

        return true;
    }

    /* save command given */
    if (!strcmp(temp_str, CMD_STR_SAVE)) {
        /* free the temp_str as there is no other use for it */
        free(temp_str);

        /* was an output file given */
        if (!settings->file_out || !file_save(settings, history)) {
            printf("Invalid output file destination!\n");
            return true;
        }

        /* history was correctly saved */
        return true;
    }

    /* assume a play command and get its values */
    tok_str = strtok(temp_str, "_");
    if (tok_str && tok_str[0] != '\0') play_row = atoi(tok_str);
    tok_str = strtok(0, "_");
    if (tok_str && tok_str[0] != '\0') play_col = atoi(tok_str);

    /* free the temp_str */
    free(temp_str);

    if (play_col <= 0 || play_col > settings->board_cols || play_row <= 0 ||
        play_row > settings->board_rows) {
        printf("Invalid move!\n");
        return true;
    }

    if (!board_play(board, cur_player->color, play_col, play_row)) {
        printf("Invalid move!\n");
        return true;
    }

    /* process the next move after */
    settings->next_move = true;

    /* add this move to the history */
    if (history) history_add(history, command);

    return true;
}

/**
 * Play as a minimax player
 *
 * @param board_t * board Board structure pointer on which game is played
 * @param player_t * cur_player Current player to which assign the command
 * @param settings_t * settings Application settings structure pointer
 * @param history_t * history Command history to save the command into
 * @return bool Returns true on successful move, else returns false
 */
bool play_minimax(board_t *board, player_t *cur_player, settings_t *settings,
                  history_t *history) {
    unsigned int play_col = 0;
    unsigned int play_row = 0;
    char command[INPUT_SIZE];

    if (!board || !cur_player || !settings) return false;

    /* calculate minimax */
    minimax(cur_player->color, board, cur_player->depth, INT_MIN, INT_MAX, true,
            &play_col, &play_row);

    /* if the play coordinates are 0, play a pass command */
    if (play_col == 0 && play_row == 0) {
        printf("(%s:%s) > pass\n", cur_player->name, "minimax");
        settings->next_move = true;

        /* save the pass command into history */
        if (history) history_add(history, "pass\n");

        return true;
    }

    printf("(%s:%s) > %d_%d\n", cur_player->name, "minimax", play_row,
           play_col);

    /* check if play coordinates are in bounds or not */
    if (play_col <= 0 || play_col > settings->board_cols || play_row <= 0 ||
        play_row > settings->board_rows) {
        printf("Invalid move!\n");
        return true;
    }

    /* try to play the move */
    if (!board_play(board, cur_player->color, play_col, play_row)) {
        printf("Invalid move!\n");
        return true;
    }

    /* process the next move */
    settings->next_move = true;

    /* add the play command into history */
    if (history) {
        sprintf(command, "%d_%d\n", play_row, play_col);
        history_add(history, command);
    }

    return true;
}

/**
 * Play moves stored in history
 *
 * @param board_t * board Board structure pointer on which game is played
 * @param player_t * cur_player Current player to which assign the command
 * @param settings_t * settings Application settings structure pointer
 * @param history_t * history Command history to save the command into
 * @return bool Returns true on success, else returns false
 */
bool play_history(board_t *board, player_t **cur_player, settings_t *settings,
                  history_t *history) {
    unsigned int i = 0;

    if (!settings || !board || !history || !cur_player) return false;

    for (i = 0; i < history->size; ++i) {
        /* process the next move */
        next_move(settings, board, cur_player);

        if ((*cur_player)->type == PLAYER_TYPE_MINIMAX) {
            play_minimax(board, *cur_player, settings, NULL);
        } else {
            play_command(board, *cur_player, settings, NULL,
                         history->commands[i]);
        }
    }

    return true;
}

/**
 * Check if the game is over
 *
 * @param const board_t * board Board structure with game data
 * @return bool Returns true if the game is over, else returns false
 */
bool is_game_over(const board_t *board) {
    bool black_can_play = true;
    bool white_can_play = true;

    /* game over if board does not exist */
    if (!board) return true;

    /* game over if board is full */
    if (board_is_full(board)) return true;

    /* can black player play */
    board_calculate(board, PLAYER_COLOR_BLACK);
    black_can_play = board_can_play(board);

    /* can white player play */
    board_calculate(board, PLAYER_COLOR_WHITE);
    white_can_play = board_can_play(board);

    /* game over if both players cannot play */
    if (!white_can_play && !black_can_play) return true;

    /* else it is not game over */
    return false;
}
