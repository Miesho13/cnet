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

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef __linux__
#   include <arpa/inet.h>
#   include <sys/socket.h>
#   include <sys/types.h>
#   include <unistd.h>
#   include <netinet/in.h>
#   include <netdb.h>
#   include <sys/epoll.h>
#elif defined(_WIN32)
#   errors "The Windows port has not been crated yet"
#elif defined(__APPLE__) && defined(__MACH__)
#   errors "x)"
#endif

#define CNET_EPOLL_MAX_EVENT (10)
#define EVENT_POLL_SIZE (64)
#define CNET_TX_BUFFER_MAX_SIZE (4*1024)

typedef enum {
    CNET_TRANSPORT_UDP,
    CNET_TRANSPORT_TCP,
} TRANSPORT_T;

typedef struct  {
    uint8_t data[CNET_TX_BUFFER_MAX_SIZE];
    uint32_t data_size;
    char host[NI_MAXHOST];
    char port[NI_MAXSERV];
    struct sockaddr recv_sock;
    socklen_t address_len; 
} cnet_message_t;

typedef struct _cnet_context_t {
    int fd;
    TRANSPORT_T transport;
    
    // epoll
    int epollfd;
    struct epoll_event ev;
    struct epoll_event events[EVENT_POLL_SIZE];

    struct client {
        struct sockaddr dest_addr;
        socklen_t dest_addr_len;
        char host[NI_MAXHOST];
        char port[NI_MAXSERV];
    } clients_ctx;
    
    // callback
    int (*recv_callback)(struct _cnet_context_t *ctx, cnet_message_t* msg);
    int (*send_callback)(struct _cnet_context_t *ctx, cnet_message_t* msg);
    
} cnet_context_t;



int cnet_init_server(cnet_context_t *ctx, 
                     const char *uri, 
                     const char *port, 
                     TRANSPORT_T transport);
int cnet_async_open(cnet_context_t *ctx, 
                    const char *host, 
                    const char *port, 
                    TRANSPORT_T transport);
int cnet_step(cnet_context_t *ctx);

int cnet_send(cnet_context_t *ctx, uint8_t *tx, size_t tx_size);

#endif
