/*
 * This file is part of fox-and-hounds task.
 *
 * Stanisław Grams <sjg@fmdx.pl>
 * main.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <getopt.h>
#include <time.h>

#include "common.h"
#include "board.h"
#include "socket.h"
#include "gui.h"

int32_t
main (int32_t argc, char *argv[]) {
  int wins = 0;
  int turn = 0;
  board_t board;
  move_t  recv_move;
  move_t  send_move;
  pawn_type_t pawn = argc <= 1 ? (pawn_type_t)fox : (pawn_type_t)hounds;
  pawn_t  fox;

  board_init (&board);
  board_add_hounds (&board);

  gui_init (&board);
  gui_draw_board (&board);
  gui_clear_events ();

  if (pawn == 0) {
    printf ("you are a fox!\n");
    conn_srv_init ();
    move_recv (&recv_move);
    printf ("hounds joined!\n");

    srand (time(NULL));
    fox.x = BOARD_SIZE - 1;
    fox.y = rand () % BOARD_SIZE;
    if (fox.y % 2 == 1) {
      if (fox.y != BOARD_SIZE - 1) fox.y++;
      else fox.y--;
    }
    board_add_fox (&board, fox);

    send_move.px = INIT_VALUE;
    send_move.x  = fox.x;
    send_move.y  = fox.y;
    move_send (&send_move);

    turn = 1;
    // init completed for fox, it's his turn!
  } else {
    printf ("you are the hounds!\n");
    conn_cli_init (argv[1]);

    send_move.px = INIT_VALUE;
    move_send (&send_move);

    // receive a fox
    move_recv (&recv_move);
    fox.x    = recv_move.x;
    fox.y    = recv_move.y;
    fox.type = 0;
    board_add_fox (&board, fox);

    turn = 0;
    // init completed, it's fox turn, wait in the loop
  }

  // game loop
  while (1<2)
  {
    gui_clear_events ();
    gui_draw_board (&board);
    // your turn - try to send a move
    if (turn == 1) {
      gui_handle_events (&board, &send_move, pawn);
      move_send (&send_move);
      wins = board_check_state (&board);
      if (wins != -1) {
        printf ("game finished\n");
        gui_draw_end (&board, wins == pawn);
        return EXIT_SUCCESS;
      }
      turn = 0;
    }
    // wait for opponents turn
    printf ("waiting for opponent's response!\n");
    do {
      move_recv (&recv_move);
    } while (recv_move.px == INIT_VALUE);
    gui_make_move (&board, recv_move, (pawn == 0) ? 1 : 0);
    turn = 1;

    wins = board_check_state (&board);
    if (wins != -1) {
      gui_draw_end (&board, wins == pawn);
      printf ("game finished\n");
      return EXIT_SUCCESS;
    }
  }

  printf ("game finished\n");
  return EXIT_SUCCESS;
}
