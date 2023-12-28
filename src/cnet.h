#ifndef __CNET__
#define __CNET__

#include "cnet_cfg.h"

#include <stdint.h>

#ifdef __linux__
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#endif



typedef enum {
    CNET_TCP = 0,
    CNET_TLS,
    CNET_UDP,
    CNET_DTLS,

} transport_layer_t;

typedef struct {
    int sockfd;
    struct sockaddr_in sock_addr;
    
} cnet_conn_t;


/*
 * 
 */
cnet_conn_t* cnet_client_open(char *uri, uint16_t port,
                              transport_layer_t tran);
/*
 * 
 */
int  cnet_client_send(cnet_conn_t cnh, uint8_t* msg, size_t msg_len);

/*
 * 
 */
int  cnet_client_recv(cnet_conn_t cnh, uint8_t* msg, size_t msg_len);

/*
 *
 */
void cnet_client_close(cnet_conn_t cnh);


#endif
