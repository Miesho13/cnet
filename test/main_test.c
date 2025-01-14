#include "../src/cnet.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <string.h>


int main(void) {
    printf("Clinet start...");
    cnet_udp_client_ctx ctx = {0};
    int ret = cnet_udp_client_open(&ctx, "localhost:6969");
    char msg[] = "Troche XD\n";
    ret = cnet_udp_client_send(&ctx, (uint8_t*)msg, strlen(msg));
    close(ctx.sock);

    return 0;
}
