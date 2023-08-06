/**
 * @brief 串口設備應用編程
*/
#include "serial_linux_api.h"

#define _DEFAULT_SOURCE

#include <stdio.h>
#include <errno.h>
#include <inttypes.h>
#include <fcntl.h>
#include <memory.h>
#include <string.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <unistd.h>
// #include <linux/termios.h>
#include <termios.h>

int serial_linux_open(const char *dev_name) {
  int fd;
  int flags = (O_RDWR|O_NOCTTY|O_NONBLOCK);
  
  fd = open(dev_name, flags);
  if (fd < 0) {
    perror("open serial port");
    return -1;
  }

  return fd;
}

int serial_linux_config(int fd, int baudrate, int databits, char parity, int stopbits) {
  struct termios new_cfg, old_cfg;
  int speed;

  if (fd < 0) {
    return -1;
  }

  // 保存並測試現有串口配置
  if (tcgetattr(fd, &old_cfg) != 0) {
    perror("tcgetattr");
    return -1;
  }

  // 設置字符大小
  new_cfg = old_cfg;
  // 配置爲原始模式
  cfmakeraw(&new_cfg);
  new_cfg.c_cflag &= ~CSIZE;

  // 設置波特率
  switch (baudrate) {
    case 9600: {
      speed = B9600;
      break;
    }
    case 115200: {
      speed = B115200;
      break;
    }
    case 230400: {
      speed = B230400;
      break;
    }
    case 921600: {
      speed = B921600;
      break;
    }
    default : {
      return -1;
    }
  }
  cfsetispeed(&new_cfg, speed);
  cfsetospeed(&new_cfg, speed);

  //  設置數據位
  switch (databits) {
    case 7 : {
      new_cfg.c_cflag |= CS7;
      break;
    }
    case 8 : {
      new_cfg.c_cflag |= CS8;
      break;
    }
    default : {
      return -1;
    }
  }

  // 設置奇偶校驗位
  switch (parity) {
    case 'n':
    case 'N': {
      new_cfg.c_cflag &= ~PARENB;
      new_cfg.c_iflag &= ~INPCK;
      break;
    }
    case 'o':
    case 'O': {
      new_cfg.c_cflag |= (PARODD | PARENB);
      new_cfg.c_iflag |= INPCK;
      break;
    }
    case 'e':
    case 'E': {
      new_cfg.c_cflag |= PARENB;
      new_cfg.c_cflag &= ~PARODD;
      new_cfg.c_iflag |= INPCK;
      break;
    }
    default : {
      return -1;
    }
  }

  // 設置停止位
  switch (stopbits) {
    case 1: {
      new_cfg.c_cflag &= ~CSTOPB;
      break;
    }
    case 2: {
      new_cfg.c_cflag |= CSTOPB;
      break;
    }
    default : {
      return -1;
    }
  }

  // 設置等待時間和最小接受字節長度
  new_cfg.c_cc[VTIME] = 0;
  new_cfg.c_cc[VMIN] = 0;

  // 處理未接收字節
  tcflush(fd, TCIFLUSH);

  // 激活新配置
  if (tcsetattr(fd, TCSANOW, &new_cfg) != 0) {
    perror("tcsetattr");
    return -1;
  }
  
  return 0;
}