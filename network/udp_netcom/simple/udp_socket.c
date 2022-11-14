#include "udp_socket.h"

int create_socket(OBJn_t obj, const char *ip, const char *port, struct sockaddr_in *pnet_addr, socklen_t net_addrlen) {
    int sockfd;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd == -1) {
        perror("fail to creat socket");
        exit(EXIT_FAILURE);
    }

    pnet_addr->sin_family = AF_INET;
    pnet_addr->sin_port = htons((short)atoi(port));
    pnet_addr->sin_addr.s_addr = inet_addr(ip);

   if(obj == SERVER){
       if( bind(sockfd,(const struct sockaddr*) pnet_addr, net_addrlen) == -1) {
          perror("fail to bind");
          exit(EXIT_FAILURE);
       }
       printf("bind sucess.. \n");
   }

   return sockfd;
}

ssize_t recv_dgram_data(int sockfd, char *buf , int size) {
    ssize_t n;
    struct sockaddr_in peer_addr;
    int addrlen = sizeof(peer_addr);

#ifdef DEBUG 
    char debug[1024];
#endif
    n = recvfrom(sockfd, buf, size, 0, (struct sockaddr *)&peer_addr, &addrlen);
    if(n < 0){
        perror("fail to recvform");
        exit(EXIT_FAILURE);
    }

#ifdef DEBUG
    memcpy(debug, buf, n);
    debug[n] = '\0';
    printf("recv %ld bytes : %s\n", n, debug);
#endif

    printf("Sender IP : %s\n", inet_ntoa(peer_addr.sin_addr));
    printf("Sender port : %d\n", ntohs(peer_addr.sin_port));

    return n;
}

void loop_recv_dgram_data(int sockfd) {
    char recv_buf[1024];  //recv data buffer 

    while(1) {
        recv_dgram_data(sockfd, recv_buf, sizeof(recv_buf));
    }
}


ssize_t trans_dgram_data(int sockfd, char *buf, int size, struct sockaddr_in *pnet_addr, socklen_t net_addrlen) {
    ssize_t n;

    n = sendto(sockfd, buf, size, 0, (struct sockaddr *)pnet_addr, net_addrlen);
    if(n == -1) {
        perror("fail to sendto");
        exit(EXIT_FAILURE);
    }
    
    return n;
}

void loop_trans_dgram_data(int sockfd, struct sockaddr_in *pnet_addr, socklen_t net_addrlen) { 
    char trans_buf[1024];

   while(1) {
       printf(">input <<");
       fgets(trans_buf, sizeof(trans_buf), stdin);
       trans_dgram_data(sockfd, trans_buf, sizeof(trans_buf), pnet_addr, net_addrlen);
       printf("\n");
   }
}



