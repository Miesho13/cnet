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

typedef enum {
    TRANSPORT_TCP = 0,
    TRANSPORT_TLS,
    TRANSPORT_UDP,
    TRANSPORT_DTLS,
} transport_layer_t;

typedef struct {
    char *host_name;
    char *port;
} host_info;

typedef struct {
    host_info host;
    transport_layer_t transport;
    int sockfd;
    struct addrinfo server_addr_info;
} cnet_client;

typedef struct {
    host_info host;

    int sockfd;
    struct sockaddr_in server_addr; // ipv4
} cnet_server;

typedef struct {
    struct sockaddr_in sockaddr;
    socklen_t addrlen;
    size_t msg_len;
    uint8_t msg[512];

} message;

cnet_client* cnet_client_open_udp(const char* host);
int cnet_client_send(cnet_client *ctx, uint8_t* msg, size_t msg_len);

cnet_server* cnet_server_udp_init(const char *host);
int cnet_server_recv(cnet_server * ctx, message *msg);


#endif
