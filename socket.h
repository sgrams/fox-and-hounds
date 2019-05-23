/*
 * This file is part of fox-and-hounds task.
 *
 * Stanisław Grams <sjg@fmdx.pl>
 * socket.h
 */
#ifndef _FOX_SOCKET_H
#define _FOX_SOCKET_H
#include <stdint.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "common.h"

#define DEFAULT_PORT 5050

int32_t conn_srv_init ();
int32_t conn_cli_init (char *opponent_addr);
int32_t move_send (move_t *move);
int32_t move_recv (move_t *move);

#endif // _FOX_SOCKET_H
