#include "othello/board.h"
#include "othello/command.h"
#include "othello/file.h"
#include "othello/minimax.h"
#include "othello/settings.h"
#include "othello/types.h"

#include <i386/limits.h>
#include <malloc/_malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_SIZE               0xFF
#define HISTORY_BUFFER_INCREMENT 32
#define HISTORY_ELEMENT_SIZE     16

void usage(const char *exe) {
    printf("usage: %s <options>\n\n"
           "options:\n"
           "  -pb <desc>        description of the black player, "
           "\"<human/minimax>[,name][,d]\"\n"
           "      name          unique name of the player\n"
           "      d             tree depth for the minimax algorithm\n"
           "  -pw <desc>        description of the white player, same as -pb\n"
           "  -size <r> <c>     size of the playing board\n"
           "  -init <ir> <ic>   position of the initial stones on the board\n"
           "  -o <gamefile>     output gamefile\n"
           "  -i <gamefile>     input gamefile\n",
           exe);
}

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

int main(int argc, char *argv[]) {
    settings_t *settings = NULL;
    board_t *board       = NULL;
    player_t *cur_player = NULL;
    char **history       = NULL;
    char command[INPUT_SIZE];
    int i                     = 0;
    int args_status           = 0;
    unsigned int play_col     = 0;
    unsigned int play_row     = 0;
    int minimax_eval          = 0;
    unsigned int history_size = 0;
    bool other_player_passed  = false;

    /* set default settings */
    settings = (settings_t *)malloc(sizeof(settings_t));
    settings_set_default(settings);

    /* process command-line arguments */
    if ((args_status = settings_process_args(settings, argc, argv))) {
        settings_free(&settings);
        return args_status;
    }

    /* create a new board */
    board = board_create(settings->board_cols, settings->board_rows);
    if (!board) return -1;

    /* place initial stones */
    board_init(board, settings->init_col, settings->init_row);

    /* set current player to white for black to be first */
    cur_player = settings->player_white;

    /* initialize history */
    history      = (char **)malloc(sizeof(char *) * HISTORY_BUFFER_INCREMENT);
    history_size = 0;

    /* game loop */
    while (!settings->should_exit) {

        /* initialize input buffer to zeros */
        for (i = 0; i < INPUT_SIZE; ++i) command[i] = 0;

        if (settings->next_move) {
            /* switch players */
            cur_player = cur_player->color == PLAYER_COLOR_WHITE
                           ? settings->player_black
                           : settings->player_white;

            /* calculate possible moves */
            board_calculate(&board, cur_player->color);
            /* redraw the board */
            board_draw(board);

            if (board_full(board)) {
                settings->should_exit = true;
                break;
            }

            /* reset state */
            settings->next_move = false;

            /* calculate minimax */
            play_col = 0;
            play_row = 0;
            if (cur_player->type == PLAYER_TYPE_MINIMAX) {
                minimax_eval =
                    minimax(cur_player->color, board, cur_player->depth,
                            INT_MIN, INT_MAX, true, &play_col, &play_row);

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

                if (!board_play(&board, cur_player->color, play_col,
                                play_row)) {
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
        if (settings->interactive) fgets(command, INPUT_SIZE, stdin);

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
                if (!board_play(&board, cur_player->color, play_col,
                                play_row)) {
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

    /* exit application */
    for (i = 0; i < history_size; ++i) free(history[i]);
    free(history);
    board_free(&board);
    settings_free(&settings);
    return 0;
}
