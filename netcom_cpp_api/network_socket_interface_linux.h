
#ifndef __SOCKET_INTERFACE_LINUX_H__
#define __SOCKET_INTERFACE_LINUX_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <sys/types.h>	      
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>

#include <iostream>
#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

typedef enum NetworkCommDevEnum {
  NET_NULL,
  UDP_SERVER, 
  UDP_CLIENT,
  TCP_SERVER,
  TCP_CLIENT
}NetCommDevTypeDef;

class UDPSocketInterfaceLinux {
public:
  UDPSocketInterfaceLinux();
  
  ~UDPSocketInterfaceLinux();
  
  bool CreateSocket(NetCommDevTypeDef obj, const char *ip, const char *port);

  bool CloseSocket();

  bool TransToNet(uint8_t *tx_buf, uint32_t tx_buff_len, uint32_t *tx_len);

  void SetRecvCallback(std::function<void(const char *, size_t length)> callback);

  bool IsClientAck() { return is_server_recv_ack_flag_.load();}

private:
  std::thread *recv_thread_;
  long long recv_count_;
  int32_t com_sockfd_;
  NetCommDevTypeDef ncd_;
  std::atomic<bool> is_cmd_created_, recv_thread_exit_flag_, is_server_recv_ack_flag_;
  std::function<void(const char *, size_t length)> recv_callback_;
  std::string server_ip_, server_port_;
  std::string client_ip_, client_port_;

  bool IsCreated() { return is_cmd_created_.load(); }

  bool RecvFromNet(uint8_t *rx_buf , uint32_t rx_buff_len, uint32_t *rx_len);

  static void RecvThreadProc(void *param);
};


class TCPSocketInterfaceLinux {
public:
  TCPSocketInterfaceLinux();
  
  ~TCPSocketInterfaceLinux();
  
  bool CreateSocket(NetCommDevTypeDef obj, const char *ip, const char *port);

  bool CloseSocket();

  bool TransToNet(uint8_t *tx_buf, uint32_t tx_buff_len, uint32_t *tx_len);

  void SetRecvCallback(std::function<void(const char *, size_t length)> callback);

private:
  std::thread *recv_thread_;
  long long recv_count_;
  int32_t com_sockfd_;
  int32_t listend_client_sockfd_;   //// server model used
  NetCommDevTypeDef ncd_;
  std::atomic<bool> is_cmd_created_, recv_thread_exit_flag_;
  std::function<void(const char *, size_t length)> recv_callback_;

  bool IsCreated() { return is_cmd_created_.load(); }

  bool RecvFromNet(uint8_t *rx_buf , uint32_t rx_buff_len, uint32_t *rx_len);
  
  static void RecvThreadProc(void *param);
};


#endif
