#ifndef _OTHELLO_SETTINGS_H_
#define _OTHELLO_SETTINGS_H_

#include "othello/types.h"

/**
 * Create new application settings
 *
 * @return settings_t * Allocated structure pointer
 */
settings_t *settings_create();

/**
 * Process and assing values from command-line arguments
 *
 * @param settigns_t * settings Settings structure to save read data into
 * @param int argc Number of command-line arguments
 * @param char ** argv Values of command-line arguments
 * @return int Returns 0 when successfully parsed, else returns error code
 */
int settings_process_args(settings_t *settings, int argc, char **argv);

/**
 * Free the settigns_t structure
 *
 * @param settings_t ** settings Address of the settings_t structure pointer
 */
void settings_free(settings_t **settings);

#endif /* ifndef _OTHELLO_SETTINGS_H_ */
