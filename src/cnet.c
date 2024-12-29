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

cnet_server* cnet_server_udp_init(const char *host) {
    cnet_server *ctx = calloc(1, sizeof(*ctx));
    ctx->sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    host_info host_string = resolve_host(host);
    ctx->server_addr.sin_family = AF_INET;
    ctx->server_addr.sin_port = htons(atoi(host_string.port));
    ctx->server_addr.sin_addr.s_addr = INADDR_ANY;
    
    int ret = bind(ctx->sockfd, (struct sockaddr*)&ctx->server_addr, 
                   sizeof(ctx->server_addr));
    if (ret != 0) { return NULL; }
    
    return ctx;
}

int cnet_server_recv(cnet_server * ctx, message *msg) {
    assert(ctx && "cnet server can't be NULL");

    int rec = recvfrom(ctx->sockfd, msg->msg, 512, 0, 
                       (struct sockaddr*)&msg->sockaddr, &msg->addrlen);
    if (rec < 0) { return -1; }
    msg->msg_len = rec;

    return rec;
}

cnet_client* cnet_client_open_udp(const char *host) {
    cnet_client *ctx = malloc(sizeof(*ctx));

    host_info host_info = resolve_host(host);
    
    ctx->host = host_info;
    ctx->transport = TRANSPORT_UDP;
    ctx->server_addr_info.ai_family = AF_UNSPEC; 
    ctx->server_addr_info.ai_socktype = SOCK_DGRAM; 
    ctx->server_addr_info.ai_protocol = 0;
    ctx->server_addr_info.ai_flags = 0;
    
    struct addrinfo *result;
    int ret = getaddrinfo(ctx->host.host_name, ctx->host.port, 
                          &ctx->server_addr_info, &result);
    if (ret != 0) {
        fprintf(stderr, "Host address canb be resolve (%d)", ret);
        goto error;
        return NULL;
    }
    struct addrinfo *rp; 
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        ctx->sockfd = socket(rp->ai_family, rp->ai_socktype,
                                  rp->ai_protocol);

        if (ctx->sockfd == -1) continue;
        if (connect(ctx->sockfd, rp->ai_addr, rp->ai_addrlen) != -1) break;

        close(ctx->sockfd);
    }
    freeaddrinfo(result);
    return ctx;

error:
    free(ctx);
    return NULL;
}

int cnet_client_send(cnet_client *ctx, uint8_t* msg, size_t msg_len) {
    assert(ctx != NULL && "CNET must not be equal to NULL");
    return send(ctx->sockfd, msg, msg_len, 0);
}
