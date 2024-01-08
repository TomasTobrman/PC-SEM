#ifndef _OTHELLO_COMMAND_H_
#define _OTHELLO_COMMAND_H_

#include "othello/types.h"

/**
 * Interpret a given command
 *
 * @param const char * command Given command string to interpret
 * @param unsigned int * play_col Column to play when a CMD_PLAY is given
 * @param unsigned int * play_row Row to play when a CMD_PLAY is given
 * @return command_code Enum code of the interpreted command
 */
command_code command_interpret(const char *command, unsigned int *play_x,
                               unsigned int *play_y);

#endif /* ifndef _OTHELLO_COMMAND_H_ */
