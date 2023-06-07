#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, const char *argv[]) {
  int ret_execlp;
  pid_t pid;

  pid = fork();
  if (pid < 0) {
    perror("fork()");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    ret_execlp = execlp("ps", "ps", "-ef", NULL);
    if (ret_execlp < 0) {
      perror("execlp .");
      exit(EXIT_FAILURE);
    }
  } else {
    ;
  }

  return 0;
}
