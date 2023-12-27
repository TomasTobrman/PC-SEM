#ifndef _OTHELLO_COMMAND_H_
#define _OTHELLO_COMMAND_H_
typedef enum { CMD_NONE, CMD_QUIT, CMD_SAVE, CMD_PASS } command_code;
command_code command_interpret(const char *command);
#endif /* ifndef _OTHELLO_COMMAND_H_ */
