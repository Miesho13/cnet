#include <stdio.h>

typedef void* (*task_function)(void*);

typedef struct task {
    task_function run;
    struct task *next;
} task_t;

typedef struct {

} async_context;

void cnet_async_init();

void cnet_async_step();

int main() {
    printf("Async test\n");
    return 0;
}
