#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../src/cnet.h"




int recv_callback_imp(message_descriptor *msg) {
    printf("test\n");
}

int main() {
    printf("Async test\n");

    async_cnet_hanler_t hcnet;
    async_cnet_init(&hcnet);

    async_recv(&hcnet, recv_callback_imp);
    async_cnet_run(&hcnet);
    return 0;
}
