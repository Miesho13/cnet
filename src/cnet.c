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

typedef enum { 
    END_TASK,
    CONTINUE_TASK,
} _EVENT;

void async_cnet_init(async_cnet_hanler_t *hcnet) {
    hcnet->task_q.head = NULL;
    hcnet->task_q.count = 0;
    hcnet->app_run = true;
}

static inline size_t _task_queue_count(async_cnet_hanler_t *hcnet) {
    return hcnet->task_q.count;
}

static inline int _task_call(task *htask) {
    return htask->ctx.call(htask->ctx.arg);
}

static inline void _task_next(task *htask) {
    htask = htask->next;
}

static inline void _task_remove(task_queue *queue, task *htask) {
    if (queue->count > 1) {
        htask->prev->next = htask->next;
        htask->next->prev = htask->prev;
        free(htask);
    }
    else if (queue->count == 1) {
        free(htask);
    }
    queue->count--;
}

static inline task* _allock_task() {
    return calloc(1, sizeof(task));
}

static inline void _push_task(task_queue *htask, task_context new_ctx_task) {
    task *new_task = _allock_task();
    memcpy(&new_task->ctx, &new_ctx_task, sizeof(new_task->ctx));
    
    if (htask->count == 0) {
        htask->head = new_task;
        htask->count++;
        return;
    }

    task *first = htask->head;
    task *last = htask->head;
    for (uint32_t i = 0; i < htask->count - 1; i++) { last = last->next; } 
    
    new_task->next = first;
    first->prev = new_task;
    last->next = new_task;
    new_task->prev = last;

    htask->head = new_task;
    htask->count++;
}

void async_cnet_run(async_cnet_hanler_t *hcnet) {
    task *current = hcnet->task_q.head;
    while (hcnet->app_run) {
        if (_task_queue_count(hcnet) == 0) {
            usleep(1);
            continue;
        }

        int ret = _task_call(current);
        current = current->next;
        
        if (ret == END_TASK) {
            _task_remove(&hcnet->task_q, current->prev);
        } 
    }
}


