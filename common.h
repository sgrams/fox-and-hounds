/*
 * This file is part of fox-and-hounds task.
 *
 * Stanisław Grams <sjg@fmdx.pl>
 * common.h
 */
#ifndef _FOX_COMMON_H
#define _FOX_COMMON_H
#include <stdint.h>

#define BOARD_SIZE (uint8_t)8
#define INIT_VALUE (uint8_t)0xFF

typedef enum {fox=0, hounds=1, empty=2} pawn_type_t;

typedef struct pawn {
  uint8_t     x;
  uint8_t     y;
  pawn_type_t type;
} pawn_t;

typedef struct move {
  // previous position
  uint8_t px;
  uint8_t py;
  // current position
  uint8_t x;
  uint8_t y;
} move_t; // 32-bit

typedef uint8_t board_t[BOARD_SIZE][BOARD_SIZE];

#endif // _FOX_COMMON_H
