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
#include <stdlib.h>
#include <stdbool.h> 
#include <string.h>
#include <stdio.h>
#include <assert.h>

#ifdef __linux__
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <unistd.h>
    #include <netinet/in.h>
    #include <netdb.h>
#elif defined(_WIN32)
    #errors "The Windows port has not been crated yet"
#elif defined(__APPLE__) && defined(__MACH__)
    #errors "x)"
#endif

static inline bool prv_is_ip4(const char * ip) {
    return inet_pton(AF_INET, ip, NULL);
}

host_info resolve_host(const char *host) {
    host_info ret = {0};

    char *port_seperator = strchr(host, ':');
    ret.port = malloc(strlen(port_seperator));
    strcpy(ret.port, port_seperator+1);
    
    uint32_t host_len = port_seperator - host;
    ret.host_name = malloc(host_len + 1);
    memcpy(ret.host_name, host, host_len);
    ret.host_name[host_len] = '\0';

    return ret;
}

int cnet_udp_client_open(cnet_udp_client_ctx *ctx, const char *host) {
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    
    host_info host_info = resolve_host(host);

    struct addrinfo *res;
    int status = getaddrinfo(host_info.host_name, host_info.port, &hints, &res);
    if (status != 0) { return -1; }
    
    for (struct addrinfo *p = res; p != NULL; p = p->ai_next) {
        ctx->sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (ctx->sock < 0) continue;

        if (connect(ctx->sock, p->ai_addr, p->ai_addrlen) == 0) {
            ctx->server_info = p;
            return 0;
        }

        close(ctx->sock);
    }

    freeaddrinfo(res);
    return -1; 
}

int cnet_udp_client_send(cnet_udp_client_ctx *ctx, uint8_t *msg, 
                         size_t msg_len) {
    return send(ctx->sock, msg, msg_len, 0);
}

int cnet_udp_client_recv(cnet_udp_client_ctx *ctx, char *buffer, size_t buffer_size) {
    int len = recv(ctx->sock, buffer, buffer_size, 0);
    if (len < 0) {
        perror("Receive failed");
        return -1;
    }
    return len;
}

cnet_server* cnet_server_udp_init(const char *host) {
    cnet_server *ctx = calloc(1, sizeof(*ctx));
    ctx->sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    host_info host_string = resolve_host(host);
    ctx->server_addr.sin_family = AF_INET;
    ctx->server_addr.sin_port = htons(atoi(host_string.port));
    ctx->server_addr.sin_addr.s_addr = INADDR_ANY;

    // TODO(marcin.ryzewski): Add the porper way to handle the error. Fuck that
    //                        for now
    int ret = bind(ctx->sockfd, (struct sockaddr*)&ctx->server_addr, 
                   sizeof(ctx->server_addr));
    if (ret != 0) { return NULL; }
    
    return ctx;
}

int cnet_server_recvfrom(cnet_server *ctx, incomming_msg *msg) {
    assert(ctx && "cnet server can't be NULL");

    int rec = recvfrom(ctx->sockfd, msg->msg_buff, 512, 0, 
                       (struct sockaddr*)&msg->sockaddr, &msg->addrlen);
    if (rec < 0)  return -1;

    msg->msg_size = rec;
    return rec;
}

int cnet_server_sendto(cnet_server *ctx, const struct sockaddr *addr, 
                     const socklen_t addr_len, uint8_t *buffer, size_t buffer_len) {
    ssize_t ret = sendto(ctx->sockfd, buffer, buffer_len, 0,
                        addr, addr_len);
    return ret;
}

