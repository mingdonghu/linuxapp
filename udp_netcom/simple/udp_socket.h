#ifndef __UDP_SOCKET_H__
#define __UDP_SOCKET_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>	      
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#ifdef __cplusplus
extern "C" {
#endif

#define DEBUG

typedef enum CommDevEnum
{
    SERVER = 0x00, 
    CLIENT = 0x01
}OBJn_t;


int create_socket(OBJn_t obj, const char *ip, const char *port, struct sockaddr_in *pnet_addr, socklen_t net_addrlen);

void loop_trans_dgram_data(int sockfd, struct sockaddr_in *pnet_addr, socklen_t net_addrlen);

void loop_recv_dgram_data(int sockfd);

#ifdef __cplusplus
}
#endif

#endif
