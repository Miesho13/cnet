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
} host_descriptor;

typedef struct {
    host_descriptor host;

    transport_layer_t transport;

    int sockfd;
    struct addrinfo server_addr_info;
} cnet_conn_t;


/*
 * 
 */
cnet_conn_t* cnet_client_open_tcp(const char *uri, uint16_t port,
                              transport_layer_t tran);
/*
 * Use this functions for crate context for udp comunication.
 */
cnet_conn_t* cnet_client_open_udp(const char *host);
cnet_conn_t* cnet_client_close_udp(const char *uri, uint16_t port,
                              transport_layer_t tran);

/*
 * 
 */
int  cnet_client_send(cnet_conn_t *cnh, uint8_t* msg, size_t msg_len);

/*
 * 
 */
int  cnet_client_recv(cnet_conn_t cnh, uint8_t* msg, size_t msg_len);

/*
 *
 */
void cnet_client_close(cnet_conn_t cnh);


#endif
