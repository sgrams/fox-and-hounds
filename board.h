/*
 * This file is part of fox-and-hounds task.
 *
 * Stanisław Grams <sjg@fmdx.pl>
 * board.h
 */
#ifndef FOX_BOARD_H
#define FOX_BOARD_H
#include "common.h"

int32_t
board_init (board_t *board);

int32_t
board_set_pawn (board_t *board, pawn_t pawn);

int32_t
board_make_move (board_t *board, move_t move, pawn_type_t pawn_type);

int32_t
board_check_state (board_t *board);

int32_t
board_check_move (board_t *board, move_t move);

int32_t
board_add_hounds (board_t *board);

int32_t
board_add_fox (board_t *board, pawn_t fox);
#endif // FOX_BOARD_H
