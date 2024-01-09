#ifndef _OTHELLO_OTHELLO_H_
#define _OTHELLO_OTHELLO_H_

#include "othello/board.h"
#include "othello/file.h"
#include "othello/history.h"
#include "othello/minimax.h"
#include "othello/settings.h"
#include "othello/types.h"

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
                history_t **history);

/**
 * Change the current player to the next one and recalculate the board
 *
 * @param settings_t * settings Application settings structure pointer
 * @param board_t * board Board structure pointer to recalculate
 * @param player_t ** cur_player Address of current player variable to change
 * @return bool Returns true if the game should end, else returns false
 */
bool next_move(settings_t *settings, board_t *board, player_t **cur_player);

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
                  history_t *history, const char *command);

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
                  history_t *history);

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
                  history_t *history);

/**
 * Check if the game is over
 *
 * @param const board_t * board Board structure with game data
 * @return bool Returns true if the game is over, else returns false
 */
bool is_game_over(const board_t *board);

#endif /* ifndef _OTHELLO_OTHELLO_H_ */
