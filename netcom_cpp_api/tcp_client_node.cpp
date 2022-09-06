/**
 * @brief udp client 
*/
#include "network_socket_interface_linux.h"

class ClientNode {
private:
  /* data */

public:
  ClientNode() {

  }
  ~ClientNode() {

  }

  void ReadCall(const char *byte, size_t len) {
    printf("recv:");
    for (int i = 0; i < (int)len; i++) {
      printf("%x ", (uint8_t)byte[i]);
    }
    printf("\n");
  }
};

int main(int argc, const char *argv[]) {

  if(argc != 3) {
    printf("<./xx> <Server IP> <Server port>\n");
    exit(EXIT_FAILURE);
  }
  printf("terminal input: <server IP>:%s, <server Port>:%s\n", argv[1], argv[2]);

  ClientNode *c_node = new ClientNode();
  TCPSocketInterfaceLinux *client_tcp_network = new TCPSocketInterfaceLinux();

  client_tcp_network->SetRecvCallback(std::bind(&ClientNode::ReadCall, c_node, std::placeholders::_1, std::placeholders::_2)); 

  bool result = client_tcp_network->CreateSocket(TCP_CLIENT, argv[1], argv[2]);
  if (!result) {
    perror("client: create socket is fail.");
    exit(EXIT_FAILURE);
  }
  printf("client: create socket is ok.\n");
  uint8_t trans_buf[10] = {0xaa, 0xaa, 0x44, 0x55, 0x66};
  uint32_t tx_len;
  printf("trans data..\n");

  while (1) {
    if (!client_tcp_network->TransToNet((uint8_t *)trans_buf, sizeof(trans_buf), &tx_len)) {
      perror("client: trans info is fail.");
      exit(EXIT_FAILURE);
    }
    printf("send:");
    for (int i = 0; i < (int)tx_len; i++) {
      printf("%x ", trans_buf[i]);
    }
    printf("\n");
    usleep(1000);
  }

  client_tcp_network->CloseSocket();
  delete c_node;
  delete client_tcp_network;

  return 0;
}
