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
#ifdef __linux__
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#endif


cnet_conn_t* cnet_client_open(const char *uri, uint16_t port,
                              transport_layer_t tran) { 

    cnet_conn_t *conn_ret = (cnet_conn_t*)malloc(sizeof(cnet_conn_t)); 
    
    switch(tran) {
        case CNET_TCP:
            conn_ret->sockfd = socket(AF_INET, SOCK_STREAM, 0);

            conn_ret->sock_addr.sin_family = AF_INET;
            int result = inet_pton(AF_INET, uri, &conn_ret->sock_addr.sin_addr);
            if (result <= 0) {
                
            }


            break;

        default:
            free(conn_ret);
            conn_ret = NULL; 
            break;
    }


    return conn_ret;
}
