#include "othello/board.h"
#include "othello/bool.h"
#include "othello/command.h"
#include "othello/minimax.h"
#include "othello/player.h"
#include "othello/settings.h"

#include <stdio.h>
#include <stdlib.h>

#define INPUT_SIZE 0xFF

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

int main(int argc, char *argv[]) {
    settings_t *settings = NULL;
    board_t *board       = NULL;
    player_t *cur_player = NULL;
    node_t *minimax      = NULL;
    char command[INPUT_SIZE];
    int i                 = 0;
    int args_status       = 0;
    unsigned int play_col = 0;
    unsigned int play_row = 0;

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

    /* initialize input buffer to zeros */
    for (i = 0; i < INPUT_SIZE; ++i) command[i] = 0;

    /* set current player to black for white to be first */
    cur_player = settings->player_black;

    /* game loop */
    while (!settings->should_exit) {
        if (settings->next_move) {
            /* switch players */
            cur_player = cur_player->color == PLAYER_COLOR_WHITE
                           ? settings->player_black
                           : settings->player_white;

            /* calculate possible moves */
            board_calculate(&board, cur_player->color);
            /* redraw the board */
            board_draw(board);

            /* reset state */
            settings->next_move = false;

            /* calculate minimax */
            if (cur_player->type == PLAYER_TYPE_MINIMAX) {
                minimax_free(&minimax);
                minimax = (node_t *)malloc(sizeof(node_t));
                if (!minimax) {
                    settings->should_exit = true;
                    continue;
                }
                minimax->board      = board_copy(board);
                minimax->weight     = 0;
                minimax->play_col   = 0;
                minimax->play_row   = 0;
                minimax->n_children = 0;
                minimax->children   = NULL;
                minimax_generate(cur_player->depth, &minimax,
                                 cur_player->color);
                minimax_draw(minimax);
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
                settings->next_move = true;
                break;
            case CMD_QUIT:
                settings->should_exit = true;
                break;
            case CMD_SAVE:
                if (!settings->file_out)
                    printf("Invalid output file destination!\n");
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
                break;
            default:
                printf("Invalid move!\n");
        };
    }

    /* exit application */
    board_free(&board);
    settings_free(&settings);
    return 0;
}
