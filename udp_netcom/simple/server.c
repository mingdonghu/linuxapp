/*
 * 功能:
 * 服务端接收
 * */
#include <stdio.h>
#include <sys/types.h>	       /* See NOTES */
#include <sys/socket.h>
#include <string.h>
#include </usr/include/netinet/in.h>
typedef enum
{
    server = 0, client = 1
}OBJn_t;



extern int create_socket(OBJn_t obj, const char *ip, const char *port, 
              struct sockaddr_in *pnet_addr, socklen_t net_addrlen);

extern void loop_recv_dgram_data(int sockfd);

int main(int argc, const char *argv[])
{
    int sockfd;
    struct sockaddr_in net_addr;
   
    if(argc < 3)
    {
        printf("<./xx> <IP> <port>");
        exit(EXIT_FAILURE);
    }
    sockfd = create_socket(server, argv[1], argv[2], &net_addr, sizeof(net_addr));
    printf("sockfd : %d\n", sockfd);
     
    loop_recv_dgram_data(sockfd);

    return 0;
}
