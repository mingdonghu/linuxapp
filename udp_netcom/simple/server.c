/**
 * @brief udp server
*/
#include "udp_socket.h"


int main(int argc, const char *argv[])
{
    int sockfd;
    struct sockaddr_in net_addr;
   
    if(argc < 3)
    {
        printf("<./xx> <IP> <port>\n");
        exit(EXIT_FAILURE);
    }
    sockfd = create_socket(SERVER, argv[1], argv[2], &net_addr, sizeof(net_addr));
    printf("sockfd : %d\n", sockfd);
     
    loop_recv_dgram_data(sockfd);

    return 0;
}
