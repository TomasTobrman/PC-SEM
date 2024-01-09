#include "othello/othello.h"

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
    history_t *history   = NULL;
    char command[INPUT_SIZE];
    int status = 0;

    /* create everything for the game */
    if ((status = create_game(argc, argv, &settings, &board, &history)))
        return status;

    /* process command history */
    if (history) play_history(board, &cur_player, settings, history);
    else history = history_create();

    /* game loop */
    while (!settings->should_exit) {

        /* initialize input buffer to zeros */
        memset(command, 0, INPUT_SIZE);

        /* process next move */
        if (settings->next_move && next_move(settings, board, &cur_player))
            break;

        /* minimax player's turn */
        if (cur_player->type == PLAYER_TYPE_MINIMAX) {
            play_minimax(board, cur_player, settings, history);
            continue;
        }

        /* get user input */
        printf("(%s:%s) > ", cur_player->name, cur_player->type_str);
        fgets(command, INPUT_SIZE, stdin);

        /* play the inputed command */
        play_command(board, cur_player, settings, history, command);
    }

    /* print end score */
    printf("Game Over!\n%s (Black): %d\n%s (White): %d\n",
           settings->player_black->name, board_count(board, PLAYER_COLOR_BLACK),
           settings->player_white->name,
           board_count(board, PLAYER_COLOR_WHITE));

    /* save game */
    if (settings->file_out) file_save(settings, history);

    /* exit application */
    history_free(&history);
    board_free(&board);
    settings_free(&settings);
    return 0;
}
