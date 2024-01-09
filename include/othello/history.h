#ifndef _OTHELLO_HISTORY_H_
#define _OTHELLO_HISTORY_H_

#include "othello/types.h"

/**
 * Create a new history structure pointer
 *
 * @return history_t * Allocated history structure pointer
 */
history_t *history_create();

/**
 * Add a command to history
 *
 * @param history_t * history History structure pointer
 * @param const char * command String command to add to history
 * @return bool Returns true on success, else returns false
 */
bool history_add(history_t *history, const char *command);

/**
 * Free history structure pointer from memory
 *
 * @param history_t ** history Address of a history structure pointer to free
 */
void history_free(history_t **history);

#endif /* ifndef _OTHELLO_HISTORY_H_ */
