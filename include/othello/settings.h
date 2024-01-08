#ifndef _OTHELLO_SETTINGS_H_
#define _OTHELLO_SETTINGS_H_

#include "othello/types.h"

/**
 * Set default values to all settings
 *
 * @param settings_t * settings Settings structure pointer
 * @return bool Returns true if successful, else false
 */
bool settings_set_default(settings_t *settings);

/**
 * Read the initial stone placement from command-line arguments
 *
 * @param settigns_t * settings Settings structure to save read data into
 * @param int argc Number of command-line arguments
 * @param char ** argv Values of command-line arguments
 * @param int * i Current index in the command-line arguments array
 * @return bool Returns true if the player was successfully read, else false
 */
int settings_process_args(settings_t *settings, int argc, char **argv);

/**
 * Process and assing values from command-line arguments
 *
 * @param settigns_t * settings Settings structure to save read data into
 * @param int argc Number of command-line arguments
 * @param char ** argv Values of command-line arguments
 * @return int Returns 0 when successfully parsed, else returns error code
 */
void settings_free(settings_t **settings);

#endif /* ifndef _OTHELLO_SETTINGS_H_ */
