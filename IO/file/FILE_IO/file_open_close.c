#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[]) {
  int fd;
  
  fd = open("test.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);
  if (fd < 0) {
    perror("open test.txt is fail.");
    exit(EXIT_FAILURE);
  }
  
  if (close(fd) == -1) {
    perror("close test.txt is fail.");
    exit(EXIT_FAILURE);
  }

  return 0;
}
