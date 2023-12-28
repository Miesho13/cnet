
#include "cnet.h"

#include <stdlib.h>

cnet_conn_t* cnet_client_open(const char *uri, uint16_t port,
                              transport_layer_t tran) { 

    cnet_conn_t *conn_ret = (cnet_conn_t*)malloc(sizeof(cnet_conn_t)); 
    
    switch(tran) {
        case CNET_TCP:
            conn_ret->sockfd = socket(AF_INET, SOCK_STREAM, 0);

            conn_ret->sock_addr.sin_family = AF_INET;
            int result = inet_pton(AF_INET, uri, &conn_ret->sock_addr.sin_addr);
            if (result <= 0) {
                
            }


            break;

        default:
            free(conn_ret);
            conn_ret = NULL; 
            break;
    }


    return conn_ret;
}
