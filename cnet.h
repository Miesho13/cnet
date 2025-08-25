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

typedef enum {
    CNET_TRANSPORT_UDP,
    CNET_TRANSPORT_TCP,
} TRANSPORT_T;

typedef struct {
    int fd;
    TRANSPORT_T transport;

    // epoll
    int epollfd;
    struct epoll_event ev;

    
} cnet_context_t;

int cnet_init_server(cnet_context_t *ctx, 
                     const char *uri, 
                     const char *port, 
                     TRANSPORT_T transport);
int cnet_step(cnet_context_t *ctx);
int cnet_async_recv();
int cnet_async_send();

#endif
