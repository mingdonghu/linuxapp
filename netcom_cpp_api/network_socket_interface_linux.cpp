
#include "network_socket_interface_linux.h"

#define MAX_RECV_BUF_LEN 4096

UDPSocketInterfaceLinux::UDPSocketInterfaceLinux() : recv_thread_(nullptr),
  recv_count_(0),
  com_sockfd_(-1),
  ncd_(NET_NULL),
  is_cmd_created_(false),
  recv_thread_exit_flag_(true),
  is_server_recv_ack_flag_(false),
  recv_callback_(nullptr) {

}

UDPSocketInterfaceLinux::~UDPSocketInterfaceLinux() {
  CloseSocket();
}

bool UDPSocketInterfaceLinux::CreateSocket(NetCommDevTypeDef obj, const char *ip, const char *port) {

  if (IsCreated()) {
    perror("repeat create socket.");
    return false;
  }

  if ((ip == nullptr) || (port == nullptr)) {
    perror("input ip address or port number is nuyll");
    return false;
  }

  if ((UDP_CLIENT == obj) || (UDP_SERVER == obj)) {
    com_sockfd_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(com_sockfd_ == -1) {
      perror("fail to socket.");
      return false;
    }
  } else {
    perror("obj is null");
    return false;
  }

  struct sockaddr_in net_addr;
  int addrlen = sizeof(net_addr);
  bzero(&net_addr, sizeof(net_addr));
  net_addr.sin_family = AF_INET;
  net_addr.sin_addr.s_addr = inet_addr(ip);
  net_addr.sin_port = htons((short)atoi(port));
  
  if(obj == UDP_SERVER){
    if(-1 == bind(com_sockfd_, (const struct sockaddr*)&net_addr, (socklen_t)addrlen)) {
      perror("fail to bind.");
      if (com_sockfd_ != -1) {
        close(com_sockfd_);
        com_sockfd_ = -1;
      }   
      return false;
    }
    printf("bind sucess.. \n");
  }

  ncd_ = obj;
  server_ip_ = ip;
  server_port_ = port;

  // create recv process thread.
  recv_thread_exit_flag_.store(false);
  recv_thread_ = new std::thread(RecvThreadProc, this);
  is_cmd_created_.store(true);

  return true;
}

bool UDPSocketInterfaceLinux::CloseSocket() {
  if (!IsCreated()) {
    return true;
  }

  recv_thread_exit_flag_.store(true);

  if (com_sockfd_ != -1) {
    close(com_sockfd_);
    com_sockfd_ = -1;
  }

  if ((recv_thread_ != nullptr) && recv_thread_->joinable()) {
    recv_thread_->join();
    delete recv_thread_;
    recv_thread_ = nullptr;
  }

  is_cmd_created_.store(false);

  return true;
}

bool UDPSocketInterfaceLinux::RecvFromNet(uint8_t *rx_buf , uint32_t rx_buff_len, uint32_t *rx_len) {
  static timespec timeout = {0, (long)(100 * 1e6)};
  int32_t len = -1;
  struct sockaddr_in sender_addr_inf;
  bzero(&sender_addr_inf, sizeof(sender_addr_inf));
  int addrlen = sizeof(struct sockaddr_in);

  if (IsCreated()) {
    ////  setting and init fd table
    fd_set recv_fds;
    FD_ZERO(&recv_fds);
    FD_SET(com_sockfd_, &recv_fds);
    int ret = pselect(com_sockfd_ + 1, &recv_fds, NULL, NULL, &timeout, NULL);
    if (ret < 0) {
      // select was interrputed
      if (errno == EINTR) {
        return false;
      }
    } else if (ret == 0) { 
      //  timeout
      return false;
    }

    if (FD_ISSET(com_sockfd_, &recv_fds)) {
      len = (int32_t)recvfrom(com_sockfd_, rx_buf, rx_buff_len, 0, 
          (struct sockaddr *)&sender_addr_inf, (socklen_t *)&addrlen);
      if ((len != -1) && (rx_len != nullptr)) {
        *rx_len = len;

        std::string sender_ip = inet_ntoa(sender_addr_inf.sin_addr);
        uint16_t sender_port = ntohs(sender_addr_inf.sin_port);
        char sender_port_str[10] = {0};
        snprintf(sender_port_str, 10, "%d", sender_port);

        if (ncd_ == UDP_SERVER) {
          if (!is_server_recv_ack_flag_.load()) { /////   保存与服务端通信的客户端IP和端口号，经支持一对一
            client_ip_ = sender_ip;
            client_port_ = sender_port_str;
            is_server_recv_ack_flag_.store(true);
          }
        }

        printf("Recv lens:%d\n", len);
        printf("Sender IP:%s\n", sender_ip.c_str());
        printf("Sender port:%s\n", sender_port_str);
      }
    }
  }

  return ((len == -1) ? false : true);
}

bool UDPSocketInterfaceLinux::TransToNet(uint8_t *tx_buf, uint32_t tx_buff_len, uint32_t *tx_len) {
  int32_t len = -1;
  struct sockaddr_in recver_net_addr;
  int addrlen = sizeof(recver_net_addr);
  bzero(&recver_net_addr, sizeof(recver_net_addr));
  recver_net_addr.sin_family = AF_INET;
 
  if (IsCreated()) {
    if (ncd_ == UDP_CLIENT) {
      recver_net_addr.sin_addr.s_addr = inet_addr(server_ip_.c_str());
      recver_net_addr.sin_port = htons((short)atoi(server_port_.c_str()));
    } else if (ncd_ == UDP_SERVER) {
      if (!is_server_recv_ack_flag_.load()) {
        return false;
      } else {
        recver_net_addr.sin_addr.s_addr = inet_addr(client_ip_.c_str());
        recver_net_addr.sin_port = htons((short)atoi(client_port_.c_str()));
      }
    }

    len = (int32_t)sendto(com_sockfd_, tx_buf, tx_buff_len, 0, 
        (struct sockaddr *)&recver_net_addr, (socklen_t)addrlen);
    if ((len != -1) && (tx_len != nullptr)) {
      *tx_len = len;
      printf("send lens:%d\n", len);
      printf("Reciver IP:%s\n", inet_ntoa(recver_net_addr.sin_addr));
      printf("Reciver port:%d\n", ntohs(recver_net_addr.sin_port));
    }
  }

  return ((len == -1) ? false : true);
}

void UDPSocketInterfaceLinux::SetRecvCallback(std::function<void(const char *, size_t length)> callback) {
    recv_callback_ = callback;
}  

bool UDPSocketInterfaceLinux::IsCreated() {
  return is_cmd_created_.load(); 
}

void UDPSocketInterfaceLinux::RecvThreadProc(void *param) {
  UDPSocketInterfaceLinux *cmd_if = (UDPSocketInterfaceLinux *)param;
  char *recieve_buff = new char[MAX_RECV_BUF_LEN + 1];

  while (!cmd_if->recv_thread_exit_flag_.load()) {
    uint32_t recved = 0;
    bool res = cmd_if->RecvFromNet((uint8_t *)recieve_buff, MAX_RECV_BUF_LEN, &recved);
    if (res && recved) {
      cmd_if->recv_count_ += recved;
      if (cmd_if->recv_callback_ != nullptr) {
        cmd_if->recv_callback_(recieve_buff, recved);
      }
    }
  }

  delete[] recieve_buff;
}


TCPSocketInterfaceLinux::TCPSocketInterfaceLinux() : recv_thread_(nullptr),
  recv_count_(0),
  com_sockfd_(-1),
  listend_client_sockfd_(-1),
  is_cmd_created_(false),
  recv_thread_exit_flag_(false),
  recv_callback_(nullptr) {

}
  
TCPSocketInterfaceLinux::~TCPSocketInterfaceLinux() {
  CloseSocket();
}
  
bool TCPSocketInterfaceLinux::CreateSocket(NetCommDevTypeDef obj, const char *ip, const char *port) {
  if (is_cmd_created_ == true) {
    perror("repeat create socket.");
    return false;
  }

  if ((ip == nullptr) || (port == nullptr)) {
    perror("input ip address or port number is nuyll");
    return false;
  }

  if ((TCP_CLIENT == obj) || (TCP_SERVER == obj)) {
    com_sockfd_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(com_sockfd_ == -1) {
      perror("fail to socket.");
      return false;
    }
  } else {
    perror("obj is null");
    return false;
  }

  struct sockaddr_in net_addr;
  int addrlen = sizeof(net_addr);
  bzero(&net_addr, sizeof(net_addr));
  net_addr.sin_family = AF_INET;
  net_addr.sin_addr.s_addr = inet_addr(ip);
  net_addr.sin_port = htons((short)atoi(port));

  ncd_ = obj;

  if (TCP_SERVER == obj) {
    // bind sockfd
    if (-1 == bind(com_sockfd_,(const struct sockaddr*)&net_addr,(socklen_t)addrlen)) {
      perror("fail to bind.");
      if (com_sockfd_ != -1) {
        close(com_sockfd_);
        com_sockfd_ = -1;
      }   
      return false;
    }
    printf("bind sucess.\n");

    // sockfd in listen
    if (-1 == listen(com_sockfd_, 1)) {
      perror("listen error.");
      if (com_sockfd_ != -1) {
        close(com_sockfd_);
        com_sockfd_ = -1;
      }   
      return false;
    }
    printf("wait client connect.\n");

    // wait client request.  one by one
    struct sockaddr_in client_addr;
    int addrlens = sizeof(client_addr);
    bzero(&client_addr, sizeof(client_addr));
   
    if (-1 == (listend_client_sockfd_ = accept(com_sockfd_, (struct sockaddr*)&client_addr, (socklen_t*)&addrlens))) {
      perror("accept error");
      if (com_sockfd_ != -1) {
        close(com_sockfd_);
        com_sockfd_ = -1;
      }   
      return false;
    }
    printf("connect_client_fd  : %d\n", listend_client_sockfd_);
    printf("client port : %d\n", ntohs(client_addr.sin_port));
    printf("client ip   : %s\n", inet_ntoa(client_addr.sin_addr));

  } else if (TCP_CLIENT == obj) {
    // connect to server
    if (-1 == connect(com_sockfd_, (const struct sockaddr*)&net_addr, (socklen_t)addrlen)) {
      perror("connect server error.");
      if (com_sockfd_ != -1) {
        close(com_sockfd_);
        com_sockfd_ = -1;
      }   
      return false;
    }
    printf("connect server success.");
  }

   // create recv process thread.
  recv_thread_exit_flag_.store(false);
  recv_thread_ = new std::thread(RecvThreadProc, this);
  is_cmd_created_.store(true);

  return true;
}

bool TCPSocketInterfaceLinux::CloseSocket() {
  if (!IsCreated()) {
    return true;
  }

  recv_thread_exit_flag_.store(true);

  if (com_sockfd_ != -1) {
    close(com_sockfd_);
    com_sockfd_ = -1;
  }

  if (listend_client_sockfd_ != -1) {
    close(listend_client_sockfd_);
    listend_client_sockfd_ = -1;
  }

  if ((recv_thread_ != nullptr) && recv_thread_->joinable()) {
    recv_thread_->join();
    delete recv_thread_;
    recv_thread_ = nullptr;
  }

  is_cmd_created_.store(false);

  return true;
}

bool TCPSocketInterfaceLinux::RecvFromNet(uint8_t *rx_buf , uint32_t rx_buff_len, uint32_t *rx_len) {
  static timespec timeout = {0, (long)(100 * 1e6)};
  int32_t len = -1;

  if (IsCreated()) {
    if (TCP_CLIENT == ncd_) {
      //// io multiplexing
      fd_set recv_fds;
      FD_ZERO(&recv_fds);
      FD_SET(com_sockfd_, &recv_fds);
      int ret = pselect(com_sockfd_ + 1, &recv_fds, NULL, NULL, &timeout, NULL);
      if (ret < 0) {
        // select was interrputed
        if (errno == EINTR) {
          return false;
        }
      } else if (ret == 0) {
        // timeout
        return false;
      }

      if (FD_ISSET(com_sockfd_, &recv_fds)) {
        len = (int32_t)recv(com_sockfd_, rx_buf, rx_buff_len, 0);
        if ((len != -1) && (rx_len != nullptr)) {
          *rx_len = len;
        }
      }
    } else if (TCP_SERVER == ncd_) {
      fd_set recv_fds;
      FD_ZERO(&recv_fds);
      FD_SET(listend_client_sockfd_, &recv_fds);
      int ret = pselect(listend_client_sockfd_ + 1, &recv_fds, NULL, NULL, &timeout, NULL);
      if (ret < 0) {
        // select was interrputed
        if (errno == EINTR) {
          return false;
        }
      } else if (ret == 0) {
        // timeout
        return false;
      }

      if (FD_ISSET(listend_client_sockfd_, &recv_fds)) {
        len = (int32_t)recv(listend_client_sockfd_, rx_buf, rx_buff_len, 0);
        if ((len != -1) && (rx_len != nullptr)) {
          *rx_len = len;
        }
      }
    }
  }

  return ((len == -1) ? false : true);
}

bool TCPSocketInterfaceLinux::TransToNet(uint8_t *tx_buf, uint32_t tx_buff_len, uint32_t *tx_len) {
  int32_t len = 1;

  if (IsCreated()) {
    if (TCP_CLIENT == ncd_) {
      len = (int32_t)send(com_sockfd_, tx_buf, tx_buff_len, 0);
      if ((len != -1) && (tx_len == nullptr)) {
        *tx_len = len;
      }
    } else if (TCP_SERVER == ncd_) {
      len = (int32_t)send(listend_client_sockfd_, tx_buf, tx_buff_len, 0);
      if ((len != -1) && (tx_len != nullptr)) {
        *tx_len = len;
      }
    }
  }

  return ((len == -1) ? false : true);
}

void TCPSocketInterfaceLinux::SetRecvCallback(std::function<void(const char *, size_t length)> callback) {
  recv_callback_ = callback;
}

bool TCPSocketInterfaceLinux::IsCreated() {
  return is_cmd_created_.load();
}

void TCPSocketInterfaceLinux::RecvThreadProc(void *param) {
  TCPSocketInterfaceLinux *cmd_if = (TCPSocketInterfaceLinux *)param;
  char *recieve_buff = new char[MAX_RECV_BUF_LEN + 1];

  while (!cmd_if->recv_thread_exit_flag_.load()) {
    uint32_t recved = 0;
    bool res = cmd_if->RecvFromNet((uint8_t *)recieve_buff, MAX_RECV_BUF_LEN, &recved);
    if (res && recved) {
      cmd_if->recv_count_ += recved;
      if (cmd_if->recv_callback_ != nullptr) {
        cmd_if->recv_callback_(recieve_buff, recved);
      }
    }
  }

  delete[] recieve_buff;
}