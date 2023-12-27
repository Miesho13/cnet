#include "../src/cnet.h"
#include <stdint.h>
#include <string.h>

int main(void) {
    
    cnet_peer_ctx_t *client = cnet_connect("localhost", 4242, TCP);

    int byte_send = cnet_send(client, http_req, sizeof(http_req));

    uint8_t *http_respone = (uint8_t*)malloc(255);
    int byte_rcev = cnet_rcv(client, &http_respone);

    printf("%s", (char*)http_respone);
    cnet_close(client);
}
