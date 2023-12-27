#include "othello/command.h"

#include <bits/stdint-uintn.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define CMD_STR_SAVE "save"
#define CMD_STR_QUIT "quit"
#define CMD_STR_PASS "pass"

command_code command_interpret(const char *command) {
  uint16_t i = 0;
  char *str = 0;
  command_code code = CMD_NONE;

  /* copy the command string */
  str = (char *)malloc(strlen(command) + 1);
  if (!str)
    return CMD_NONE;
  strcpy(str, command);

  /* remove trailing '\n' */
  str[strcspn(str, "\r\n")] = 0;

  /* input to lowercase */
  for (i = 0; str[i]; i++)
    str[i] = tolower(str[i]);

  /* get command code */
  if (!strcmp(str, CMD_STR_QUIT))
    code = CMD_QUIT;
  else if (!strcmp(str, CMD_STR_PASS))
    code = CMD_PASS;
  else if (!strcmp(str, CMD_STR_SAVE))
    code = CMD_SAVE;

  /* free allocated string and return the right code */
  free(str);
  return code;
}
