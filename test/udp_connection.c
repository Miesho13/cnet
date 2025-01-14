#include "../src/cnet.h"
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>


void handle_sigint(int sig) {
    // its only for symple test perpous!
    exit(0);
}

#define INCOMMING_MSG_CONSTRUCTOR(BUFF, LEN) (incomming_msg){.max_buff_len = LEN, .msg_buff = BUFF, .msg_size = 0}

int main() {
    signal(SIGINT, handle_sigint); 

    cnet_server *ctx = cnet_server_udp_init("localhost:6969");
    
    uint8_t buff[512] = {0};
    incomming_msg msg = INCOMMING_MSG_CONSTRUCTOR(buff, 512);
//    uint8_t buff[512] = {0};
//    incomming_msg msg = {0};
//    msg.msg_buff = buff;
//    msg.max_buff_len = 512;
    while (1) {
        cnet_server_recvfrom(ctx, &msg);
        msg.msg_buff[msg.msg_size] = '\0';
        printf("[%s:%d] %s\n",inet_ntoa(msg.sockaddr.sin_addr), ntohs(msg.sockaddr.sin_port), msg.msg_buff);
        cnet_server_sendto(ctx, (struct sockaddr*)&msg.sockaddr, 
                           msg.addrlen, msg.msg_buff, msg.msg_size);
    }

    close(ctx->sockfd);
    return 0;
}
