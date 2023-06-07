#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <unistd.h>

#include "file_lock_set.h"

int main(int argc, char const *argv[]) { 
  int fd = open("test.txt", O_RDWR | O_CREAT, 0666);
  if (fd < 0) {
    perror("open error");
    exit(EXIT_FAILURE);
  }

  file_lock_set(fd, F_RDLCK); // 给文件加读锁
  getchar(); // 等待用户输入，阻塞进程
  file_lock_set(fd, F_UNLCK); // 给文件解锁
  getchar(); // 等待用户输入，阻塞进程
  close(fd);

  return 0; 
}
