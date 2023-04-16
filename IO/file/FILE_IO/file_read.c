/*
 * 功能：
 * open file and read file content"
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
  ssize_t rnum;
  char rbuffer[10];

  // 1、open file
  fd_file = open("./file.txt", O_RDONLY);
  if (fd_file == -1) {
    perror("open file.txt");
    exit(EXIT_FAILURE);
  }

  // 2、read file
  rnum = read(fd_file, rbuffer, 10);
  printf("rnum = %ld\n", rnum);
  if (rnum == -1) {
    printf("read failure : %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
  printf("%s\n", rbuffer);

  // 3、close file
  if (close(fd_file) == -1) {
    perror("close file.txt");
    exit(EXIT_FAILURE);
  }

  return 0;
}
