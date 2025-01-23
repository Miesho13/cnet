#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../src/cnet.h"

typedef int (*task_function)(void*);

typedef struct task {
    void *arg;
    bool done;
    task_function step;
    struct task *next;
} task_t;

typedef struct {
    task_t *head;
    size_t task_count; 
} async_context;

void cnet_async_add_task(async_context *ctx, task_function callback, void *arg) {
    task_t *new = calloc(1, sizeof(task_t));
    new->step = callback;
    new->arg = arg;
    new->next = NULL;
    
    if (ctx->head == NULL) {
        ctx->head = new;
    }else {
        task_t *iter = ctx->head;
        while (iter->next != NULL) { iter = iter->next; }
        iter->next = new;
    }

    ctx->task_count++;
}

void cnet_async_done(async_context *ctx, task_t *done) {
    task_t *inc = ctx->head;
    
    int pose = 0;
    while (inc != done) {
        pose++; 
        inc = inc->next;
    }
    
    inc = ctx->head;
    for (int i = 0; i < pose; i++) { inc = inc->next; }
    if (done == ctx->head) {
        ctx->task_count--;
        if (ctx->task_count != 0) { 
            ctx->head = inc->next;
        }
    } else if (pose == ctx->task_count){
        inc->next = NULL;
    } else {
        inc = inc->next->next;
    }

    free(done);
}

void cnet_async_init(async_context *ctx) {
    ctx->head = NULL;
    ctx->task_count = 0;
}

void cnet_async_step(async_context *ctx) {
    task_t *current_task = ctx->head;
    while (ctx->task_count != 0) {
        if (current_task->step(current_task->arg) == 0) {
            task_t *done = current_task;
            current_task = current_task->next;
            cnet_async_done(ctx, done);
        } else {
            current_task = current_task->next;   
        }

        if (current_task == NULL) {
            current_task = ctx->head;
        }
    }
}

// -------------------------------------

typedef int (*recv_callback)(void*);
typedef int (*send_callback)(void*);
typedef int (*init_callback)(void*);
typedef int (*end_callback)(void*);

typedef struct {
    recv_callback recv;
    send_callback send;
    init_callback init;
    end_callback  end;
} async_callback;

typedef struct {
    int fd;
    struct sockaddr_in server_addr;
    async_context async_ctx;
} async_server_context;


int cnet_async_server_init(async_server_context *ctx, const char *host) {
    cnet_async_init(&ctx->async_ctx);

    ctx->fd = socket(AF_INET, SOCK_DGRAM, 0);
    
    host_info host_string = resolve_host(host);
    ctx->server_addr.sin_family = AF_INET;
    ctx->server_addr.sin_port = htons(atoi(host_string.port));
    ctx->server_addr.sin_addr.s_addr = INADDR_ANY;

    // TODO(marcin.ryzewski): Add the porper way to handle the error. Fuck that
    //                        for now
    int ret = bind(ctx->fd, (struct sockaddr*)&ctx->server_addr, 
                   sizeof(ctx->server_addr));
    if (ret != 0) { return NULL; }
    
    return 0;
}

int main() {
    printf("Async test\n");
    
//    async_context ctx = {0};
//    cnet_async_init(&ctx);
//        
//    count_arg a = {0, 24};
//    cnet_async_add_task(&ctx, count, &a);
//
//    count_arg b = {6, 44};
//    cnet_async_add_task(&ctx, count, &b);
//    
//    if (list_comming) {
//
//    }

    return 0;
}
