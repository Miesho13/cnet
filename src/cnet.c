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

host_descriptor resolve_host(const char *host) {
    host_descriptor ret = {0};

    char *port_seperator = strchr(host, ':');
    ret.port = malloc(strlen(port_seperator));
    strcpy(ret.port, port_seperator+1);
    
    uint32_t host_len = port_seperator - host;
    ret.host_name = malloc(host_len + 1);
    memcpy(ret.host_name, host, host_len);
    ret.host_name[host_len] = '\0';

    return ret;
}

static inline bool prv_is_ip4(const char * ip) {
    return inet_pton(AF_INET, ip, NULL);
}

cnet_conn_t* cnet_client_open_udp(const char *host) {
    cnet_conn_t *cnet_ctx = malloc(sizeof(*cnet_ctx));

    host_descriptor host_desc = resolve_host(host);
    
    cnet_ctx->host = host_desc;
    cnet_ctx->transport = TRANSPORT_UDP;
    cnet_ctx->server_addr_info.ai_family = AF_UNSPEC; 
    cnet_ctx->server_addr_info.ai_socktype = SOCK_DGRAM; 
    cnet_ctx->server_addr_info.ai_protocol = 0;
    cnet_ctx->server_addr_info.ai_flags = 0;
    
    struct addrinfo *result;
    int ret = getaddrinfo(cnet_ctx->host.host_name, cnet_ctx->host.port, 
                          &cnet_ctx->server_addr_info, &result);
    if (ret != 0) {
        fprintf(stderr, "Host address canb be resolve (%d)", ret);
        goto error;
        return NULL;
    }
    struct addrinfo *rp; 
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        cnet_ctx->sockfd = socket(rp->ai_family, rp->ai_socktype,
                                  rp->ai_protocol);

        if (cnet_ctx->sockfd == -1) continue;
        if (connect(cnet_ctx->sockfd, rp->ai_addr, rp->ai_addrlen) != -1) break;

        close(cnet_ctx->sockfd);
    }
    freeaddrinfo(result);
    return cnet_ctx;

error:
    free(cnet_ctx);
    return NULL;
}

int cnet_client_send(cnet_conn_t *cnh, uint8_t* msg, size_t msg_len) {
    assert(cnh != NULL && "CNET must not be equal to NULL");
    return send(cnh->sockfd, msg, msg_len, 0);
}
