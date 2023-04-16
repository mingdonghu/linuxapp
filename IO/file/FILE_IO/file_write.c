/*
 * 功能：
 * open file and write content"
 */
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, const char *argv[]) {
  int fd_file;
  ssize_t wnum;
  char wbuffer[] = "hello world!";

  // 1、open file
  fd_file = open("./file.txt", O_WRONLY);
  if (fd_file == -1) {
    perror("open file.txt");
    exit(EXIT_FAILURE);
  }

  // 2、write file
  wnum = write(fd_file, wbuffer, 10);
  if (wnum == -1) {
    printf("write failure : %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  // 3、close file
  if (close(fd_file) == -1) {
    perror("close file.txt");
    exit(EXIT_FAILURE);
  }

  return 0;
}
