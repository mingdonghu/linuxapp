#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>


int main(int argc, char const *argv[])
{
  FILE* file_stream = NULL;

  file_stream = fopen("./1.txt", "r");
  if (file_stream == NULL) {
    perror("fopen ./1.txt r failed");
    printf("info: %s\n", strerror(errno));
    // 重新以 w+ 打开1.txt，若不存在将创建
    file_stream = fopen("./1.txt", "w+");
    if (file_stream == NULL) {
      perror("fopen ./1.txt w+ failed exit..");
      printf("info: %s\n", strerror(errno));
      exit(EXIT_FAILURE);
    }
    printf("info: open and create 1.txt file.\n");
  }

  if (fclose(file_stream) == EOF) {
    perror("fclose failed.");
    exit(EXIT_FAILURE);
  }

  // 验证一个进程打开的文件数有限制，包含3个标准流(stdout,stdin,stderr)，一共为1024个
  int count = 0;
  while (count < 10000) {
    file_stream = fopen("./1.txt", "r");
    if (file_stream == NULL) {
      break;
    }
    count++;
  }
  printf("count value: %d\n", count);

  return 0;
}
