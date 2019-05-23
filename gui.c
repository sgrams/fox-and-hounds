/*
 * This file is part of fox-and-hounds task.
 *
 * Stanisław Grams <sjg@fmdx.pl>
 * gui.c
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/X.h>
#include <string.h>
#include "common.h"
#include "board.h"
#include "gui.h"

// gui global variables
Display   *display;
Visual    *visual;
Window    window;
Colormap  colormap;
GC        gc;
XGCValues gcvalues;
XColor    black, white, red, yellow, green; // colors
XEvent    event;
int32_t   depth, screen;
XSetWindowAttributes windowattributes;

int32_t
gui_init ()
{
  display = XOpenDisplay  ("");
  screen  = DefaultScreen (display);
  visual  = DefaultVisual (display, screen);
  depth   = DefaultDepth  (display, screen);

  windowattributes.override_redirect = False;
  windowattributes.background_pixel  = XWhitePixel (display, 0);

  window = XCreateWindow (display, XRootWindow (display, screen),
    100, 100, 480, 480, 10, depth, InputOutput, visual,
    CWBackPixel | CWOverrideRedirect, &windowattributes);
  XSelectInput (display, window, ExposureMask | ButtonPressMask | KeyPressMask);

  colormap = DefaultColormap (display, screen);
  XAllocNamedColor (display, colormap, "black",  &black,  &black);
  XAllocNamedColor (display, colormap, "white",  &white,  &white);
  XAllocNamedColor (display, colormap, "red",    &red,    &red);
  XAllocNamedColor (display, colormap, "green",  &green,  &green);
  XAllocNamedColor (display, colormap, "yellow", &yellow, &yellow);

  XStoreName (display, window,"Fox and Hounds");
  XMapWindow (display, window);
  gc = DefaultGC (display, screen);

  return 0;
}

int32_t
gui_draw_board (board_t *board)
{
  pawn_t pawn;
  int32_t i, j;

  if (NULL == board) {
    return -1;
  }

  XClearWindow (display, window);

  for (i = 0; i < BOARD_SIZE; ++i)
  {
    for (j = 0; j < BOARD_SIZE; ++j)
    {
      XSetForeground (display, gc, black.pixel);
      if (0 == (j+i) % 2) {
        XSetForeground (display, gc, red.pixel);
      }
      XFillRectangle (display, window, gc, 60 * j, 60 * i, 60, 60);

      if (board[0][i][j] != (uint8_t) empty) {
        pawn.x = i;
        pawn.y = j;
        pawn.type = (pawn_type_t)board[0][i][j];
        gui_draw_pawn (pawn);
      }
    }
  }

  XFlush (display);
  return 0;
}

int32_t
gui_handle_events (board_t *board, move_t *move, pawn_type_t pawn_type)
{
  int status = 0;
  static int clicks = 0;

  if (NULL == board) {
    return -1;
  }

  while (status != 1) {
    XNextEvent (display, &event);
    switch (event.type) {
      case Expose:
      gui_draw_board (board);
      break;
      case ButtonPress:
      // first clicks
      if (clicks == 0) {
        clicks = 1;
        move->py = event.xbutton.x / 60;
        move->px = event.xbutton.y / 60;

        if (board[0][move->px][move->py] == (uint8_t)empty ||
            board[0][move->px][move->py] != (uint8_t)pawn_type)
        {
          clicks = 0;
        }
      }
      else { // second click
        move->y = event.xbutton.x / 60;
        move->x = event.xbutton.y / 60;

        if (board_check_move (board, *move)) {
          gui_make_move (board, *move, pawn_type);
          status = 1; // move is executed
        }
        else {
          fprintf (stderr, "Invalid move!\n");
        }
        clicks = 0;
      }
    }
  }

  return status;
}

int32_t
gui_make_move (board_t *board, move_t move, pawn_type_t pawn)
{
  board_make_move (board, move, pawn);
  gui_draw_board (board);

  return 0;
}

void
gui_clear_events ()
{
  XSync (display, True);
}

int32_t
gui_draw_end (board_t *board, uint8_t win)
{
  gui_draw_board (board);

  while (1<2) {
    if (win) {
      gui_draw_board (board);
      XSetForeground (display, gc, white.pixel);
      XDrawString (display, window, gc, 240 - strlen ("You win!") / 2, 240, "You win!", strlen ("You win!"));
      XNextEvent (display, &event);
      if (event.type == KeyPress) {
        break;
      }
    }
    else {
      gui_draw_board (board);
      XSetForeground (display, gc, white.pixel);
      XDrawString (display, window, gc, 241, 240 - strlen ("You lose!"), "You lose!", strlen ("You lose!"));
      XNextEvent (display, &event);
      if (event.type == KeyPress) {
        break;
      }
    }
  }
  return -1;
}

int32_t
gui_draw_pawn (pawn_t pawn) {
  if (pawn.type == empty) {
    return -1;
  } else if (pawn.type == fox) {
    XSetForeground (display, gc, yellow.pixel);
  } else {
    XSetForeground (display, gc, green.pixel);
  }

  // draw a pawn
  XFillArc (display, window, gc, pawn.y * 60, pawn.x * 60, 60, 60, 0, 360 * 64);
  return 0;
}
