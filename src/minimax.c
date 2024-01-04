#include "othello/minimax.h"
#include "othello/board.h"
#include "othello/types.h"

#include <stdio.h>
#include <stdlib.h>

void minimax_generate(int depth, node_t **root, player_color color,
                      bool opponent) {
    unsigned int i     = 0;
    node_t *child      = NULL;
    node_t **temp_root = NULL;

    if (depth <= 0 || !root || !*root || !(*root)->board ||
        !(*root)->board->moves)
        return;

    for (i = 0; i < (*root)->board->size; ++i) {
        if (!(*root)->board->moves[i]) continue;

        ++(*root)->n_children;

        temp_root = (node_t **)realloc((*root)->children,
                                       (*root)->n_children * sizeof(node_t *));

        child = (node_t *)malloc(sizeof(node_t));
        if (!temp_root || !child) {
            --((*root)->n_children);
            break;
        }

        (*root)->children = temp_root;

        child->board    = board_copy((*root)->board);
        child->weight   = child->board->moves[i];
        child->play_col = (i % child->board->width) + 1;
        child->play_row = (i / child->board->width) + 1;

        board_play(&child->board, color, child->play_col, child->play_row);
        board_calculate(&child->board, color);

        minimax_generate(depth - 1, &child, color, true);

        (*root)->children[(*root)->n_children - 1] = child;
    }
}

void minimax_free(node_t **root) {
    if (!root || !*root) return;
    if ((*root)->board) board_free(&(*root)->board);
    if ((*root)->children) minimax_free((*root)->children);
    free(*root);
    *root = NULL;
}

static void minimax_draw_child(node_t *root, int depth) {
    int i = 0;

    if (!root) return;

    for (i = 1; i < depth; ++i) printf("|    ");
    printf("|-- [%d] %d_%d\n", root->weight, root->play_row, root->play_col);

    if (!root->children || !root->n_children) return;
    for (i = 0; i < root->n_children; ++i) {
        minimax_draw_child(root->children[i], depth + 1);
    }
}

void minimax_draw(node_t *root) {
    int i = 0;

    if (!root) return;
    printf("minimax\n");

    if (!root->children || !root->n_children) return;
    for (i = 0; i < root->n_children; ++i) {
        minimax_draw_child(root->children[i], 1);
    }
}
