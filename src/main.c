#include "othello/board.h"
#include "othello/bool.h"
#include "othello/command.h"
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
    char command[INPUT_SIZE];
    int i           = 0;
    int args_status = 0;

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
    while (!settings->shouldExit) {
        /* switch players and redraw the board */
        if (settings->nextMove) {
            cur_player = cur_player->color == PLAYER_COLOR_WHITE
                           ? settings->player_black
                           : settings->player_white;
            board_calculate(board, cur_player->color);
            board_draw(board);
            board_draw_moves(board);
            settings->nextMove = false;
        }

        /* get user input */
        printf("(%s:%s) > ", cur_player->name,
               cur_player->type == PLAYER_TYPE_HUMAN ? "human" : "minimax");
        if (settings->interactive) fgets(command, INPUT_SIZE, stdin);

        /* interpret the given command */
        /* TODO: Player moves */
        switch (command_interpret(command)) {
            case CMD_PASS:
                settings->nextMove = true;
                continue;
            case CMD_QUIT:
                settings->shouldExit = true;
                break;
            case CMD_SAVE:
                if (!settings->file_out)
                    printf("Invalid output file destination!\n");
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
