#include "../src/cnet.h"
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>


void handle_sigint(int sig) {
    // its only for symple test perpous!
    exit(0);
}


int main() {
    signal(SIGINT, handle_sigint); 

    cnet_server *ctx = cnet_server_udp_init("localhost:6969");
    message msg = {0};

    while (1) {
        cnet_server_recv(ctx, &msg);
        msg.msg[msg.msg_len] = '\0';
        printf("[%s:%d] %s\n",inet_ntoa(msg.sockaddr.sin_addr), ntohs(msg.sockaddr.sin_port), msg.msg);
    }

    close(ctx->sockfd);
    return 0;
}
