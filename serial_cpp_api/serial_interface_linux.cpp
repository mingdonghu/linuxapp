#include "serial_interface_linux.h"

#define USE_NO_STD_BAUDRATE  0

#define MAX_ACK_BUF_LEN 4096

SerialInterfaceLinux::SerialInterfaceLinux()
    : rx_thread_(nullptr), rx_count_(0), read_callback_(nullptr) {
  com_handle_ = -1;
  com_baudrate_ = 0;
}

SerialInterfaceLinux::~SerialInterfaceLinux() { Close(); }

bool SerialInterfaceLinux::Open(std::string &port_name, uint32_t com_baudrate) {
  int flags = (O_RDWR | O_NOCTTY | O_NONBLOCK);

  com_handle_ = open(port_name.c_str(), flags);
  if (-1 == com_handle_) {
    printf("SerialInterfaceLinux::Open open error, errno:%s",strerror(errno));
    return false;
  }

  com_baudrate_ = com_baudrate;

#if (USE_NO_STD_BAUDRATE == 1)
// 非標準波特率設置
  struct asmtermios::termios2 options;
  if (ioctl(com_handle_, _IOC(_IOC_READ, 'T', 0x2A, sizeof(struct asmtermios::termios2)), &options)) {
    printf("TCGETS2 first error, errno:%s",strerror(errno));
    if (com_handle_ != -1) {
      close(com_handle_);
      com_handle_ = -1;
    }
    return false;
  }

  options.c_cflag |= (tcflag_t)(CLOCAL | CREAD | CS8);
  options.c_cflag &= (tcflag_t) ~(CSTOPB | PARENB);
  options.c_lflag &= (tcflag_t) ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ISIG | IEXTEN);  //|ECHOPRT
  options.c_oflag &= (tcflag_t) ~(OPOST);
  options.c_iflag &= (tcflag_t) ~(IXON | IXOFF | INLCR | IGNCR | ICRNL | IGNBRK);

  options.c_cflag &= ~CBAUD;
  options.c_cflag |= BOTHER;
  options.c_ispeed = this->com_baudrate_;
  options.c_ospeed = this->com_baudrate_;

  options.c_cc[VMIN] = 0;
  options.c_cc[VTIME] = 0;

  if (ioctl(com_handle_, _IOC(_IOC_WRITE, 'T', 0x2B, sizeof(struct asmtermios::termios2)), &options)) {
    printf("TCSETS2 error, errno:%s",strerror(errno));
    if (com_handle_ != -1) {
      close(com_handle_);
      com_handle_ = -1;
    }
    return false;
  }

  if (ioctl(com_handle_, _IOC(_IOC_READ, 'T', 0x2A, sizeof(struct asmtermios::termios2)), &options)) {
    printf("TCGETS2 second error, errno:%s",strerror(errno));
    if (com_handle_ != -1) {
      close(com_handle_);
      com_handle_ = -1;
    }
    return false;
  }

  printf("Actual BaudRate reported:%d",options.c_ospeed);

#else
// get port options
  struct termios options;
  if (-1 == tcgetattr(com_handle_, &options)) {
    if (com_handle_ != -1) {
      close(com_handle_);
      com_handle_ = -1;
    }
    std::cout << "CmdInterfaceLinux::Open tcgetattr error!" << std::endl;
    return false;
  }

  options.c_cflag |= (tcflag_t)(CLOCAL | CREAD | CS8 | CRTSCTS);
  options.c_cflag &= (tcflag_t) ~(CSTOPB | PARENB | PARODD);
  options.c_lflag &= (tcflag_t) ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL |
                                  ISIG | IEXTEN);  //|ECHOPRT
  options.c_oflag &= (tcflag_t) ~(OPOST);
  options.c_iflag &=
      (tcflag_t) ~(IXON | IXOFF | INLCR | IGNCR | ICRNL | IGNBRK);

  options.c_cc[VMIN] = 0;
  options.c_cc[VTIME] = 0;

  cfsetispeed(&options, B230400);

  if (tcsetattr(com_handle_, TCSANOW, &options) < 0) {
    if (com_handle_ != -1) {
      close(com_handle_);
      com_handle_ = -1;
    }
    std::cout << "CmdInterfaceLinux::Open tcsetattr error!" << std::endl;
    return false;
  }


#endif


  tcflush(com_handle_, TCIFLUSH);

  rx_thread_exit_flag_ = false;
  rx_thread_ = new std::thread(RxThreadProc, this);
  is_cmd_opened_ = true;

  return true;
}

bool SerialInterfaceLinux::Close() {
  if (is_cmd_opened_ == false) {
    return true;
  }

  rx_thread_exit_flag_ = true;

  if (com_handle_ != -1) {
    close(com_handle_);
    com_handle_ = -1;
  }

  if ((rx_thread_ != nullptr) && rx_thread_->joinable()) {
    rx_thread_->join();
    delete rx_thread_;
    rx_thread_ = NULL;
  }

  is_cmd_opened_ = false;

  return true;
}

bool SerialInterfaceLinux::ReadFromIO(uint8_t *rx_buf, uint32_t rx_buf_len,
                                   uint32_t *rx_len) {
  static timespec timeout = {0, (long)(100 * 1e6)};
  int32_t len = -1;

  if (IsOpened()) {
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(com_handle_, &read_fds);
    int r = pselect(com_handle_ + 1, &read_fds, NULL, NULL, &timeout, NULL);
    if (r < 0) {
      // Select was interrupted
      if (errno == EINTR) {
        return false;
      }
    } else if (r == 0) {  // timeout
      return false;
    }

    if (FD_ISSET(com_handle_, &read_fds)) {
      len = (int32_t)read(com_handle_, rx_buf, rx_buf_len);
      if ((len != -1) && rx_len) {
        *rx_len = len;
      }
    }
  }
  return len == -1 ? false : true;
}

bool SerialInterfaceLinux::WriteToIo(const uint8_t *tx_buf, uint32_t tx_buf_len,
                                  uint32_t *tx_len) {
  int32_t len = -1;

  if (IsOpened()) {
    len = (int32_t)write(com_handle_, tx_buf, tx_buf_len);
    if ((len != -1) && tx_len) {
      *tx_len = len;
    }
  }
  return len == -1 ? false : true;
}

void SerialInterfaceLinux::RxThreadProc(void *param) {
  SerialInterfaceLinux *cmd_if = (SerialInterfaceLinux *)param;
  char *rx_buf = new char[MAX_ACK_BUF_LEN + 1];
  while (!cmd_if->rx_thread_exit_flag_.load()) {
    uint32_t readed = 0;
    bool res = cmd_if->ReadFromIO((uint8_t *)rx_buf, MAX_ACK_BUF_LEN, &readed);
    if (res && readed) {
      cmd_if->rx_count_ += readed;
      if (cmd_if->read_callback_ != nullptr) {
        cmd_if->read_callback_(rx_buf, readed);
      }
    }
  }

  delete[] rx_buf;
}
