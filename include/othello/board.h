#ifndef _OTHELLO_BOARD_H_
#define _OTHELLO_BOARD_H_

#include "othello/types.h"

/**
 * Create a new board
 *
 * @param unsigned int width Number of columns of the board
 * @param unsigned int height Number of rows of the board
 * @return board_t * Structure pointer to the allocated board, NULL when an
 * error occurs
 */
board_t *board_create(unsigned int width, unsigned int height);

/**
 * Make a copy of a board_t structure
 *
 * @param const board_t * original The original board structure to copy
 * @return board_t * Copy of the original board_t structure
 */
board_t *board_copy(const board_t *original);

/**
 * Place the initial stones to the board
 *
 * @param board_t * board A board_t structure
 * @param unsigned int col Column of the top left stone
 * @param unsigned int row Row of the top left stone
 * @return bool Returns true if stones could be placed, else returns false
 */
bool board_init(const board_t *board, unsigned int col, unsigned int row);

/**
 * Free the board_t struct
 *
 * @param board_t **board Address of a board_t pointer
 */
void board_free(board_t **board);

/**
 * Draw the board to stdout
 *
 * @param const board_t * board A board_t struct width data to draw
 */
void board_draw(const board_t *board);

/**
 * Calculate available moves for a player
 *
 * @param const board_t * board Board structure with game data
 * @param player_color color The player for which moves should be calculated
 */
void board_calculate(const board_t *board, player_color color);

/**
 * Play a move on the board
 *
 * @param const board_t * board Board structure with game data
 * @param player_color color Player which is playing the move
 * @param unsigned int col Played column
 * @param unsigned int row Played row
 * @return bool Returns true if the move was played, else returns false
 */
bool board_play(const board_t *board, player_color color, unsigned int col,
                unsigned int row);

/**
 * Does the board have available calculated moves
 *
 * @param const board_t * board Board structure pointer with game data
 * @return bool Returns true if there are moves available, else returns false
 */
bool board_can_play(const board_t *board);

/**
 * Is the board filled with stones
 *
 * @param const board_t * board Board structure pointer with game data
 * @return bool Returns true if the board is filled, else returns false
 */
bool board_is_full(const board_t *board);

/**
 * Number of stones of a color
 *
 * @param const board_t * board Board structure pointer with game data
 * @param player_color color Color of the stones to count
 * @return Count of stones on the board of the color
 */
int board_count(const board_t *board, player_color color);

#endif /* ifndef _OTHELLO_BOARD_H_ */
