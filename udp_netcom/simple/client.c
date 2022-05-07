/**
 * @brief udp client 
*/
#include "udp_socket.h"


int main(int argc, const char *argv[])
{
    int sockfd;
    struct sockaddr_in net_addr;

    if(argc < 3)
    {
        printf("<./xx> <Server IP> <Server port>\n");
        exit(EXIT_FAILURE);
    }
    sockfd = create_socket(CLIENT, argv[1], argv[2], &net_addr, sizeof(net_addr));
    printf("sockfd : %d\n", sockfd);
     
    loop_trans_dgram_data(sockfd, &net_addr, sizeof(net_addr));

    return 0;
}
