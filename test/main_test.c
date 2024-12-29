#include "../src/cnet.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>


int main(void) {
    printf("test run...\n");
    cnet_client *send = cnet_client_open_udp("127.0.0.1:6969");
    char msg[] = "Troche XD\n";
    cnet_client_send(send, msg, strlen(msg));
    close(send->sockfd);

    return 0;
}
