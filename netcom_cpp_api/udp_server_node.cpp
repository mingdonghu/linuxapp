/**
 * @brief udp server
*/
#include "network_socket_interface_linux.h"

class ServerNode {
private:
  /* data */

public:
  ServerNode() {

  }
  ~ServerNode() {

  }

  void ReadCall(const char *byte, size_t len) {
    printf("udp-server recv:");
    for (int i = 0; i < (int)len; i++) {
      printf("%x ", (uint8_t)byte[i]);
    }
    printf("\n");
  }
};


int main(int argc, const char *argv[]) {

  if(argc != 3) {
    printf("<./xx> <IP> <port>\n");
    exit(EXIT_FAILURE);
  }
  printf("terminal input: <server IP>:%s, <server Port>:%s\n", argv[1], argv[2]);

  ServerNode *s_node = new ServerNode();
  UDPSocketInterfaceLinux *server_udp_network = new UDPSocketInterfaceLinux();

  server_udp_network->SetRecvCallback(std::bind(&ServerNode::ReadCall, s_node, std::placeholders::_1, std::placeholders::_2)); 

  bool result = server_udp_network->CreateSocket(UDP_SERVER, argv[1], argv[2]);
  if (!result) {
    perror("server: create socket is fail.");
    exit(EXIT_FAILURE);
  }
  printf("server: create socket is ok.\n");
  printf("wait client ack connect..\n");
  while (!server_udp_network->IsClientAck()) {
    usleep(1000);
  }
  uint8_t trans_buf[10] = {0xaa, 0xaa, 0x11, 0x22, 0x33};
  uint32_t tx_len;
  while (1) {
    if (!server_udp_network->TransToNet((uint8_t *)trans_buf, sizeof(trans_buf), &tx_len)) {
      perror("server: trans info is fail.");
      exit(EXIT_FAILURE);
    }
    printf("send:");
    for (int i = 0; i < (int)tx_len; i++) {
      printf("%x ", trans_buf[i]);
    }
    printf("\n");
    sleep(1);
  }

  server_udp_network->CloseSocket();
  delete s_node;
  delete server_udp_network;

  return 0;
}
