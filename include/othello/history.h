#ifndef _OTHELLO_HISTORY_H_
#define _OTHELLO_HISTORY_H_

/**
 * Add an element to a history
 *
 * @param char ** history History array
 * @param unsigned int * history_size Size of the history array
 * @param const char * element String element to add into history
 * @param char ** Returns history
 */
char **history_add(char **history, unsigned int *history_size,
                   const char *element);

#endif /* ifndef _OTHELLO_HISTORY_H_ */
