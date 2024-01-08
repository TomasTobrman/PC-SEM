#ifndef _OTHELLO_FILE_H_
#define _OTHELLO_FILE_H_

#include "othello/types.h"

#include <stdio.h>

/**
 * Save game settings and history to an output gamefile
 *
 * @param const settings_t * settings Game settings structure pointer
 * @param char ** history Game history of moves
 * @param unsigned int history_size Size of the history array
 * @return bool Returns true if the saving was successful, else returns false
 */
bool file_save(const settings_t *settings, char **history,
               unsigned int history_size);

/**
 * Load and parse an input gamefile
 *
 * @param settings_t * settings Settings structure to save parsed data into
 * @param unsigned int * hist_len Size of returned history array
 * @return History array filled with moves from the input gamefile
 */
char **file_load(settings_t *settings, unsigned int *history_size);

#endif /* ifndef _OTHELLO_FILE_H_ */
