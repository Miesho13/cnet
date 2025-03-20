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
#include <stdbool.h>
#include <stdlib.h> 
#include <assert.h>
#include <string.h>

#ifdef __linux__
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <poll.h>

#elif defined(_WIN32)
#errors "The Windows port has not been crated yet"
#elif defined(__APPLE__) && defined(__MACH__)
#errors "x)"
#endif

static inline char* _get_host(const char* input) {
    if (!input) return NULL;

    const char* colon = strchr(input, ':');
    size_t host_len = colon ? (size_t)(colon - input) : strlen(input);

    char* host = (char*)malloc(host_len + 1);
    if (!host) return NULL;

    strncpy(host, input, host_len);
    host[host_len] = '\0';

    return host;
}

static int _domain_type() {
    char* host_tmp = _get_host(host);
    if (strcmp(host_tmp, "0.0.0.0")) {
        return INADDR_ANY;
    }
    else if (strcmp(host_tmp, "localhost")) {

    }
    else if (strcmp(host_tmp, "127.0.0.1")) {

    } 
    else {

    }
    free(host_tmp);
}

void server_init(cnet *cnet_handler, const char* host) {
    int fd = socket(AF_INET, SOCK_DGRAM, 0);

}

void cnet_init(cnet *cnet_handler, size_t task_sizie) {
    cnet_handler->fds = calloc(task_sizie, sizeof(*cnet_handler->fds));
    cnet_handler->task = calloc(task_sizie, sizeof(*cnet_handler->task));
    cnet_handler->task_count = 0;
}

void async_run(cnet *cnet_handler) {
    
}








