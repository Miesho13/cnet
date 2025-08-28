/*
 *  File: cnet.c
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

#include "cnet.h"
#include "assert.h"
#ifdef __linux__
#   include <fcntl.h>
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

#define NOT_IMPL_YET() assert(false && "Do not implemented yet")

static int prv_setup_epoll(cnet_context_t *ctx) {
    ctx->epollfd = epoll_create1(0);
    if (ctx->epollfd < 0) { 
        return -1;
    }
    ctx->ev.events = EPOLLIN;
    ctx->ev.data.fd = ctx->fd;

    epoll_ctl(ctx->epollfd, EPOLL_CTL_ADD, ctx->fd, &ctx->ev);

    return 0;
}

static int prv_set_non_block(cnet_context_t *ctx) {
    int flags = fcntl(ctx->fd, F_GETFL, 0);
    if (flags == -1) {
        return -1;
    }
    return fcntl(ctx->fd, F_SETFL, flags | O_NONBLOCK);
}

static int prv_init_udp_server(cnet_context_t *ctx, const char *uri, const char *port) {
    ctx->transport = CNET_TRANSPORT_UDP;

    int ret = 0;
    struct addrinfo hints = {0};

    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_DGRAM; 
    hints.ai_flags = AI_PASSIVE; 

    struct addrinfo *res = NULL; 
    ret = getaddrinfo(uri, port, &hints, &res);
    if (ret != 0 ) { goto ret_err; }
    
    struct addrinfo *iter = NULL; 
    for (iter = res; iter != NULL; iter = iter->ai_next) {
        ctx->fd = socket(iter->ai_family, iter->ai_socktype, iter->ai_protocol);
        if (ctx->fd == -1) { continue; }

        if (bind(ctx->fd, iter->ai_addr, iter->ai_addrlen) == 0) {
            if (prv_set_non_block(ctx) == -1) { continue; }
            break;
        }

        close(ctx->fd);
    }

    prv_setup_epoll(ctx);

ret_err:
    if (res) freeaddrinfo(res);
    return ret;    
}

static int prv_init_tcp_server(cnet_context_t *ctx) {
    NOT_IMPL_YET();
    return -1;
}

int cnet_init_server(cnet_context_t *ctx, 
                     const char *uri, 
                     const char *port, 
                     TRANSPORT_T transport) {
    if (transport == CNET_TRANSPORT_UDP) {
        return prv_init_udp_server(ctx, uri, port);
    }
    else if (transport == CNET_TRANSPORT_TCP){
        return prv_init_tcp_server(ctx);
    }
    return -1;
}

static inline int prv_udp_step(cnet_context_t *ctx) {
    int event_count = epoll_wait(ctx->epollfd, ctx->events, EVENT_POLL_SIZE, -1);
    if (event_count < 0) {
        perror("epoll_wait");
        return -1;  
    }
    
    for (int event_id = 0; event_id < event_count; event_id++) {
        uint32_t event_handler = ctx->events[event_id].events;
        uint32_t related_fd = ctx->events[event_id].data.fd;

        if (event_handler & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)) {
            continue;
        }
        
        if ((event_handler & EPOLLIN) && ctx->fd == related_fd) {
            cnet_message_t msg;
            msg.address_len = sizeof(msg.recv_sock);

            msg.data_size = recvfrom(ctx->fd, msg.data, sizeof(msg.data),
               0, &msg.recv_sock, &msg.address_len);

            getnameinfo(&msg.recv_sock, msg.address_len, msg.host, 
                sizeof(msg.host), msg.port, sizeof(msg.port), 
                NI_NUMERICHOST | NI_NUMERICSERV);

            ctx->recv_callback(&msg);
        }

    }

    return 0;
}

static inline int prv_tcp_step(cnet_context_t *ctx) {
    NOT_IMPL_YET();
    return 0;
}

int cnet_step(cnet_context_t *ctx) {
    if (ctx->transport == CNET_TRANSPORT_UDP) {
        return prv_udp_step(ctx);
    }
    else if(ctx->transport == CNET_TRANSPORT_UDP) {
        return prv_tcp_step(ctx);
    }

    assert(false && "BAD TRANSPORT LAYER");
    return -1;
}























