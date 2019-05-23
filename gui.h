/*
 * This file is part of fox-and-hounds task.
 *
 * Stanisław Grams <sjg@fmdx.pl>
 * gui.h
 */
#ifndef FOX_GUI_H
#define FOX_GUI_H
#include <X11/Xlib.h>
#include <X11/X.h>

int32_t
gui_init ();

int32_t
gui_draw_board (board_t *board);

int32_t
gui_draw_pawn (pawn_t pawn);

int32_t
gui_draw_end (board_t *board, uint8_t win);

int32_t
gui_handle_events (board_t *board, move_t *move, pawn_type_t pawn_type);

void
gui_clear_events ();

int32_t
gui_make_move (board_t *board, move_t move, pawn_type_t pawn);
#endif // FOX_GUI_H
