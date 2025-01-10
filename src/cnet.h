/*
 *  File: cnet.h
 *  Author: marcin.ryzewskii@gmail.com
 *
 *  Copyright 2024 Marcin Ryzewski
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *
 *  Brief: This file contains the implementation of the CNET library.
 *         It includes functions for establishing connections, sending and
 *         receiving data, and handling errors.
 */

#ifndef __CNET__
#define __CNET__

#include "cnet_cfg.h"

#include <netdb.h>
#include <stdint.h>

#ifdef __linux__
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#endif


typedef struct {
    char *host_name;
    char *port;
} host_info;

typedef struct {
    int sock;
    struct addrinfo *server_info;
} cnet_udp_client_ctx;

/* Client context */
int cnet_udp_client_open(cnet_udp_client_ctx *ctx, const char *host);
int cnet_udp_client_send(cnet_udp_client_ctx *ctx, uint8_t *msg,  size_t msg_len);
int cnet_udp_client_recv(cnet_udp_client_ctx *ctx, char *buffer, size_t buffer_size);


typedef struct {
    host_info host;

    int sockfd;
    struct sockaddr_in server_addr; // ipv4
} cnet_server;

typedef struct {
    struct sockaddr_in sockaddr;
    socklen_t addrlen;
    size_t    max_buff_len;
    size_t    msg_size;
    uint8_t  *msg_buff;

} incomming_msg;

/* blocking polling server handler */
cnet_server* cnet_server_udp_init(const char *host);
int cnet_server_recvfrom(cnet_server *ctx, incomming_msg *msg);
int cnet_server_sendto(cnet_server *ctx, const struct sockaddr *addr,
                       const socklen_t addr_len, uint8_t *buffer, size_t buffer_len);

/* assync server */

#endif
