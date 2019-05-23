/*
 * This file is part of fox-and-hounds task.
 *
 * Stanisław Grams <sjg@fmdx.pl>
 * board.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "board.h"
#include "common.h"

int32_t
board_init (board_t *board)
{
  int32_t i, j;
  pawn_t pawn;

  for (i = 0; i < BOARD_SIZE; ++i)
  {
    for (j = 0; j < BOARD_SIZE; ++j)
    {
      pawn.x = i;
      pawn.y = j;
      pawn.type = empty;
      board_set_pawn (board, pawn);
    }
  }

  return 0;
}

int32_t
board_set_pawn (board_t *board, pawn_t pawn)
{
  if (!board || pawn.x >= BOARD_SIZE || pawn.y >= BOARD_SIZE) {
    return -1;
  }
  if (pawn.type == fox) {
    board[0][pawn.x][pawn.y] = 0;
  } else if (pawn.type == hounds) {
    board[0][pawn.x][pawn.y] = 1;
  } else {
    board[0][pawn.x][pawn.y] = 2;
  }
  return 0;
}

int32_t
board_make_move (board_t *board, move_t move, pawn_type_t type)
{
  pawn_t tmp = {.x = move.x, .y = move.y, .type = type};
  board_set_pawn (board, tmp);

  board[0][move.px][move.py] = 2;

  return 0;
}

int32_t
board_check_move (board_t *board, move_t move)
{
  // check for size
  if (move.x > BOARD_SIZE-1 || move.y > BOARD_SIZE-1) {
    return 0;
  }

  // if next field is not empty return 0
  if (board[0][move.x][move.y] != 2) {
    return 0;
  }

  // if moving to the same field return 0
  if (move.px == move.x && move.py == move.y) {
    return 0;
  }

  switch (board[0][move.px][move.py]) {
    case 0:
      if (!((move.px + 1 == move.x && move.py - 1 == move.y) ||
            (move.px + 1 == move.x && move.py + 1 == move.y) ||
            (move.px - 1 == move.x && move.py - 1 == move.y) ||
            (move.px - 1 == move.x && move.py + 1 == move.y))) {
        return 0;
			}
      break;
    case 1:
      if (!((move.px + 1 == move.x && move.py + 1 == move.y) ||
            (move.px + 1 == move.x && move.py - 1 == move.y))) {
        return 0;
      }
      break;
    // invalid move if moving an empty pawn
    case 2:
      return 0;
  }

  // return 1 if all checks passed
  return 1;
}

int32_t
board_check_state (board_t *board)
{
  int i, j;
  int found = 0;
  int blocked = 0;
  int counter = 0;
  //move_t move;

  for (i = 0; i < BOARD_SIZE && !found; ++i)
  {
    for (j = 0; j < BOARD_SIZE; ++j)
    {
      // i == x; j == y;
      if (board[0][i][j] == 0) {
        found = 1;

        if (i == 0) {
          return 0; // wins fox
        } else {
          counter = 0;
          // left upper
          if (i - 1 > 0 && j + 1 < BOARD_SIZE) {
            if (board[0][i-1][j+1] == 1) {
              blocked++;
            }
            counter++;
          }
          // right upper
          if (i + 1 < BOARD_SIZE - 1 && j + 1 < BOARD_SIZE) {
            if (board[0][i+1][j+1] == 1) {
              blocked++;
            }
            counter++;
          }
          // left lower
          if (i - 1 > 0 && j - 1 > 0) {
            if (board[0][i-1][j-1] == 1) {
              blocked++;
            }
            counter++;
          }
          // right lower
          if (i + 1 < BOARD_SIZE && j - 1 > 0) {
            if (board[0][i+1][j-1] == 1) {
              blocked++;
            }
            counter++;
          }
          if (blocked > 0 && counter > 0 && blocked == counter) {
            printf ("blocked = %i; counter = %i\n", blocked, counter);
            return 1;
          }
        }
        break;
      }
    }
  }
  return -1;
}

int32_t
board_add_hounds (board_t *board)
{
  for (int i = 1; i < BOARD_SIZE; i+=2)
  {
    board[0][0][i] = 1;
  }

  return 0;
}

int32_t
board_add_fox (board_t *board, pawn_t fox)
{
  board[0][fox.x][fox.y] = 0;
  return 0;
}
