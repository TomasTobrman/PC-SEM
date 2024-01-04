#ifndef _OTHELLO_COMMAND_H_
#define _OTHELLO_COMMAND_H_
#include "othello/types.h"
command_code command_interpret(const char *command, unsigned int *play_x,
                               unsigned int *play_y);
#endif /* ifndef _OTHELLO_COMMAND_H_ */
