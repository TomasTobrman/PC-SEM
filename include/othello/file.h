#ifndef _OTHELLO_FILE_H_
#define _OTHELLO_FILE_H_

#include "othello/types.h"

/**
 * Save game history into an output gamefile
 *
 * @param const settings_t * settings Application settings structure pointer
 * @param const history_t * history Game history with played commands
 * @return bool Returns false when an error occures, else returns true
 */
bool file_save(const settings_t *settings, const history_t *history);

/**
 * Load and parse an input gamefile
 *
 * @param settings_t * settings Settings structure to save parsed data into
 * @return History structure pointer filled with moves from the input gamefile
 */
history_t *file_load(settings_t *settings);

#endif /* ifndef _OTHELLO_FILE_H_ */
