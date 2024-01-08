#include "othello/command.h"
#include "othello/types.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define CMD_STR_SAVE "save"
#define CMD_STR_QUIT "quit"
#define CMD_STR_PASS "pass"

/**
 * Interpret a given command
 *
 * @param const char * command Given command string to interpret
 * @param unsigned int * play_col Column to play when a CMD_PLAY is given
 * @param unsigned int * play_row Row to play when a CMD_PLAY is given
 * @return command_code Enum code of the interpreted command
 */
command_code command_interpret(const char *command, unsigned int *play_col,
                               unsigned int *play_row) {
    int i             = 0;
    char *str         = 0;
    command_code code = CMD_NONE;
    char *temp_str    = 0;

    /* copy the command string */
    str = (char *)malloc(strlen(command) + 1);
    if (!str) return CMD_NONE;
    strcpy(str, command);

    /* remove trailing '\n' */
    str[strcspn(str, "\r\n")] = 0;

    /* input to lowercase */
    for (i = 0; str[i]; i++) str[i] = tolower(str[i]);

    /* get command code */
    if (!strcmp(str, CMD_STR_QUIT)) code = CMD_QUIT;
    else if (!strcmp(str, CMD_STR_PASS)) code = CMD_PASS;
    else if (!strcmp(str, CMD_STR_SAVE)) code = CMD_SAVE;
    else {
        temp_str = strtok(str, "_");
        if (temp_str && temp_str[0] != '\0') *play_row = atoi(temp_str);
        temp_str = strtok(0, "_");
        if (temp_str && temp_str[0] != '\0') *play_col = atoi(temp_str);
        code = CMD_PLAY;
    }

    /* free allocated string and return the right code */
    free(str);
    return code;
}
