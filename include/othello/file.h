#ifndef _OTHELLO_FILE_H_
#define _OTHELLO_FILE_H_
#include "othello/types.h"
#include <stdio.h>
bool file_save(const settings_t *settings, char **history,
               unsigned int history_size);
#endif /* ifndef _OTHELLO_FILE_H_ */
