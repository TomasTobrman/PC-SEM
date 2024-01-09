#include "othello/history.h"

/**
 * Create a new history structure pointer
 *
 * @return history_t * Allocated history structure pointer
 */
history_t *history_create() {
    history_t *history = (history_t *)malloc(sizeof(history_t));
    if (!history) return NULL;

    history->commands = (char **)malloc(sizeof(char *) * HISTORY_INCREMENT);
    if (!history->commands) {
        free(history);
        return NULL;
    }

    history->size = 0;

    return history;
}

/**
 * Add a command to history
 *
 * @param history_t * history History structure pointer
 * @param const char * command String command to add to history
 * @return bool Returns true on success, else returns false
 */
bool history_add(history_t *history, const char *command) {
    char **temp_arr = NULL;
    char *str       = NULL;

    if (!history || !command) return false;

    /* reallocate the history commands array to a bigger size */
    if (!((history->size + 1) % HISTORY_INCREMENT)) {
        temp_arr = (char **)realloc(history->commands,
                                    sizeof(char *) *
                                        (history->size + HISTORY_INCREMENT));
        if (!temp_arr) return false;
        history->commands = temp_arr;
    }

    str = (char *)malloc(sizeof(char) * INPUT_SIZE);
    if (!str) return false;

    str = strcpy(str, command);

    history->commands[history->size] = str;
    ++history->size;

    return true;
}

/**
 * Free history structure pointer from memory
 *
 * @param history_t ** history Address of a history structure pointer to free
 */
void history_free(history_t **history) {
    size_t i = 0;

    if (!history || !*history) return;

    if ((*history)->commands) {
        for (i = 0; i < (*history)->size; ++i) free((*history)->commands[i]);
        free((*history)->commands);
    }

    free(*history);

    *history = NULL;
}
