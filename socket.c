/*
 * This file is part of fox-and-hounds task.
 *
 * Stanisław Grams <sjg@fmdx.pl>
 * socket.c
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "common.h"
#include "socket.h"

static int32_t sockfd; // global sockfd
static struct sockaddr_in sockaddr; // global in_sockaddr

int32_t
conn_srv_init ()
{
  sockfd = socket (AF_INET, SOCK_DGRAM, 0);
  sockaddr.sin_family = AF_INET;

  sockaddr.sin_addr.s_addr = INADDR_ANY;
  sockaddr.sin_port = htons (DEFAULT_PORT);

  bind (sockfd, (struct sockaddr *) &sockaddr, sizeof (sockaddr));

  return 0;
}

int32_t
conn_cli_init (char *opponent_addr)
{
  if (!opponent_addr) {
    return -1;
  }
  sockfd = socket (AF_INET, SOCK_DGRAM, 0);
  sockaddr.sin_family = AF_INET;
  if (NULL == opponent_addr) {
    return -1;
  }

  sockaddr.sin_addr.s_addr = inet_addr (opponent_addr);
  sockaddr.sin_port = htons (DEFAULT_PORT);

  bind (sockfd, (struct sockaddr *) &sockaddr, sizeof (sockaddr));

  return 0;
}

int32_t
move_recv (move_t *move)
{
  if (NULL == move) {
    return -1;
  }
  uint32_t length, value;

  length = sizeof (sockaddr);
  recvfrom (sockfd, &value, sizeof (uint32_t), 0, (struct sockaddr *)&sockaddr, (socklen_t *)&length);
  value = ntohl (value);

  move->px = (value & 0xFF000000) >> 0x18;
  move->py = (value & 0x00FF0000) >> 0x10;
  move->x  = (value & 0x0000FF00) >> 0x08;
  move->y  = (value & 0x000000FF) >> 0x00;

  return 0;
}

int32_t
move_send (move_t *move)
{
  if (NULL == move) {
    return -1;
  }

  uint32_t length, value;
  length = sizeof (sockaddr);
  value = (move->px << 0x18) | (move->py << 0x10) | (move->x << 0x08) | (move->y << 0x00);

  value = htonl (value);
  sendto (sockfd, &value, sizeof (uint32_t), 0, (struct sockaddr *)&sockaddr, length);

  return 0;
}
