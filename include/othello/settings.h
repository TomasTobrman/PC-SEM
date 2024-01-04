#ifndef _OTHELLO_SETTINGS_H_
#define _OTHELLO_SETTINGS_H_
#include "othello/types.h"
int settings_set_default(settings_t *settings);
int settings_process_args(settings_t *settings, int argc, char *argv[]);
void settings_free(settings_t **settings);
#endif /* ifndef _OTHELLO_SETTINGS_H_ */
