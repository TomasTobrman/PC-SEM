#include "othello/board.h"
#include "othello/command.h"
#include "othello/file.h"
#include "othello/history.h"
#include "othello/minimax.h"
#include "othello/settings.h"
#include "othello/types.h"

#include <i386/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Check if the game is over
 *
 * @param const board_t * board Board structure with game data
 * @return Returns true if the game is over, else returns false
 */
static bool game_over(const board_t *board) {
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

    return false;
}

/**
 * Plays moves from a history in an input gamefile, else allocates new history
 *
 * @param char *** history Address of the history array
 * @param unsigned int * history_size Size of the history array
 * @param const settings_t * settings Settings structure pointer
 * @param board_t * board Board structure pointer with game data
 * @param player_t ** cur_player Address of the current player
 * @return bool Returns false if and error occured, else returns true
 */
static bool play_history(char ***history, unsigned int *history_size,
                         const settings_t *settings, board_t *board,
                         player_t **cur_player) {
    unsigned int i        = 0;
    unsigned int play_col = 0;
    unsigned int play_row = 0;

    if (!history || !history_size || !settings || !board || !cur_player ||
        !*cur_player)
        return false;

    /* if history was loaded from a file */
    if (*history) {
        /* play all moves in history */
        for (i = 0; i < *history_size; ++i) {
            /* switch players */
            *cur_player = (*cur_player)->color == PLAYER_COLOR_WHITE
                            ? settings->player_black
                            : settings->player_white;

            /* redraw board */
            board_draw(board);

            /* print command from history */
            printf("(%s:%s) > %s", (*cur_player)->name,
                   (*cur_player)->type == PLAYER_TYPE_HUMAN ? "human"
                                                            : "minimax",
                   (*history)[i]);

            /* interpret the command */
            switch (command_interpret((*history)[i], &play_col, &play_row)) {
                case CMD_PASS:
                    /* pass current round */
                    continue;
                case CMD_PLAY:
                    /* play command from history */
                    board_calculate(board, (*cur_player)->color);
                    board_play(board, (*cur_player)->color, play_col, play_row);
                    break;
                default:
                    continue;
            }
        }
    } else {
        /* initialize new history */
        *history = (char **)malloc(sizeof(char *) * HISTORY_BUFFER_INCREMENT);
        if (!history) return false;
        *history_size = 0;
    }

    return true;
}

/**
 * Add an element to a history
 *
 * @param char ** history History array
 * @param unsigned int * history_size Size of the history array
 * @param const char * element String element to add into history
 * @param char ** Returns history
 */
char **history_add(char **history, unsigned int *history_size,
                   const char *element) {
    char **temp_hist = NULL;
    char *str        = NULL;
    if (!history || !element) return history;

    if (!((*history_size + 1) % HISTORY_BUFFER_INCREMENT)) {
        temp_hist = (char **)realloc(
            history,
            sizeof(char *) * (*history_size + HISTORY_BUFFER_INCREMENT));
        if (!temp_hist) return history;
        history = temp_hist;
    }

    str = (char *)malloc(sizeof(char) * HISTORY_ELEMENT_SIZE);
    if (!str) return history;

    str = strcpy(str, element);

    history[*history_size] = str;
    ++(*history_size);

    return history;
}

/**
 * Application entry point and main function of the program
 *
 * @param int argc Number of command-line arguments
 * @param char ** argv Values of command-line arguments
 * @return int Returns an exit code
 */
int main(int argc, char **argv) {
    settings_t *settings = NULL;
    board_t *board       = NULL;
    player_t *cur_player = NULL;
    char **history       = NULL;
    char command[INPUT_SIZE];
    int i                     = 0;
    int args_status           = 0;
    unsigned int play_col     = 0;
    unsigned int play_row     = 0;
    unsigned int history_size = 0;
    bool other_player_passed  = false;
    const char *errors[5]     = {
        "Invalid player description!\n",
        "Invalid board size!\n",
        "Unable to place initial stones!\n",
        "Invalid output file destination!\n",
        "Invalid input file!\n",
    };

    /* set default settings */
    settings = (settings_t *)malloc(sizeof(settings_t));
    if (!settings_set_default(settings)) {
        settings_free(&settings);
        printf("something went wrong...\n");
        return -1;
    }

    /* process command-line arguments */
    if ((args_status = settings_process_args(settings, argc, argv))) {
        settings_free(&settings);
        printf("%s", errors[args_status - 1]);
        return args_status;
    }

    /* load in predefined moves */
    history = file_load(settings, &history_size);

    /* create a new board */
    board = board_create(settings->board_cols, settings->board_rows);
    if (!board) {
        for (i = 0; i < history_size; ++i) free(history[i]);
        free(history);
        settings_free(&settings);
        printf("something went wrong...\n");
        return -1;
    }

    /* place initial stones */
    if (!board_init(board, settings->init_col, settings->init_row)) {
        for (i = 0; i < history_size; ++i) free(history[i]);
        free(history);
        board_free(&board);
        settings_free(&settings);
        printf("%s", errors[2]);
        return 3;
    }

    /* set current player to white for black to be first */
    cur_player = settings->player_white;

    if (!play_history(&history, &history_size, settings, board, &cur_player)) {
        for (i = 0; i < history_size; ++i) free(history[i]);
        free(history);
        board_free(&board);
        settings_free(&settings);
        printf("something went wrong...\n");
        return -1;
    }

    /* game loop */
    while (!settings->should_exit) {

        /* initialize input buffer to zeros */
        memset(command, 0, INPUT_SIZE);

        /* process next move */
        if (settings->next_move) {
            /* is game over */
            if (game_over(board)) break;

            /* switch players */
            cur_player = cur_player->color == PLAYER_COLOR_WHITE
                           ? settings->player_black
                           : settings->player_white;

            /* calculate possible moves */
            board_calculate(board, cur_player->color);
            /* redraw the board */
            board_draw(board);

            if (board_is_full(board)) {
                settings->should_exit = true;
                break;
            }

            /* reset state */
            settings->next_move = false;

            /* calculate minimax */
            play_col = 0;
            play_row = 0;
            if (cur_player->type == PLAYER_TYPE_MINIMAX) {
                minimax(cur_player->color, board, cur_player->depth, INT_MIN,
                        INT_MAX, true, &play_col, &play_row);

                if (play_col == 0 && play_row == 0) {
                    printf("(%s:%s) > pass\n", cur_player->name, "minimax");
                    if (other_player_passed) {
                        settings->should_exit = true;
                        break;
                    }
                    other_player_passed = true;
                    settings->next_move = true;
                    history = history_add(history, &history_size, "pass\n");
                    continue;
                }

                printf("(%s:%s) > %d_%d\n", cur_player->name, "minimax",
                       play_row, play_col);

                if (play_col <= 0 || play_col > settings->board_cols ||
                    play_row <= 0 || play_row > settings->board_rows) {
                    printf("Invalid move!\n");
                    break;
                }

                if (!board_play(board, cur_player->color, play_col, play_row)) {
                    printf("Invalid move!\n");
                    break;
                }

                settings->next_move = true;
                other_player_passed = false;
                sprintf(command, "%d_%d\n", play_row, play_col);
                history = history_add(history, &history_size, command);
                continue;
            }
        }

        /* get user input */
        printf("(%s:%s) > ", cur_player->name,
               cur_player->type == PLAYER_TYPE_HUMAN ? "human" : "minimax");
        /* if (settings->interactive) fgets(command, INPUT_SIZE, stdin); */
        fgets(command, INPUT_SIZE, stdin);

        /* interpret the given command */
        switch (command_interpret(command, &play_col, &play_row)) {
            case CMD_PASS:
                if (board_can_play(board)) {
                    printf("Invalid move!\n");
                    break;
                }
                if (other_player_passed) {
                    settings->should_exit = true;
                    break;
                }
                settings->next_move = true;
                other_player_passed = true;
                history = history_add(history, &history_size, command);
                break;
            case CMD_QUIT:
                settings->should_exit = true;
                break;
            case CMD_SAVE:
                if (!settings->file_out ||
                    !file_save(settings, history, history_size)) {
                    printf("Invalid output file destination!\n");
                    break;
                }

                break;
            case CMD_PLAY:
                if (play_col <= 0 || play_col > settings->board_cols ||
                    play_row <= 0 || play_row > settings->board_rows) {
                    printf("Invalid move!\n");
                    break;
                }
                if (!board_play(board, cur_player->color, play_col, play_row)) {
                    printf("Invalid move!\n");
                    break;
                }
                settings->next_move = true;
                other_player_passed = false;
                history = history_add(history, &history_size, command);
                break;
            default:
                printf("Invalid move!\n");
        };
    }

    /* print end score */
    printf("Game Over!\n%s (Black): %d\n%s (White): %d\n",
           settings->player_black->name, board_count(board, PLAYER_COLOR_BLACK),
           settings->player_white->name,
           board_count(board, PLAYER_COLOR_WHITE));

    /* save game */
    if (settings->file_out) file_save(settings, history, history_size);

    /* exit application */
    for (i = 0; i < history_size; ++i) free(history[i]);
    free(history);
    board_free(&board);
    settings_free(&settings);
    return 0;
}
