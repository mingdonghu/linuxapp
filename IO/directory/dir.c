/*
 * 功能：
 * 统计目录中普通文件的个数
 */

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

int main(int argc, const char *argv[]) {
  DIR *dirp;
  struct dirent *mydirent;
  int count_file = 0;
  int count_dir = 0;

  // 使用命令行参数
  if (argc < 2) {
    printf("users : %s < 目录路径名 >\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // 1、打开目录文件
  dirp = opendir(argv[1]);
  if (dirp == NULL) {
    perror("opendir()");
    exit(EXIT_FAILURE);
  }

  // 2、读取目录信息
  while (1) {
    mydirent = readdir(dirp);

    // 条目末尾
    if (mydirent == NULL) {
        break;
    }

    // 普通文件
    if (mydirent->d_type == DT_REG) {
      printf("- %s\n", mydirent->d_name);
      count_file++;
      continue;
    }

    // 目录文件
    if (mydirent->d_type == DT_DIR) {
      printf("d %s\n", mydirent->d_name);
      count_dir++;
      continue;
    }
  }

  printf("count_file = %d\n", count_file);
  printf("count_dir = %d\n", count_dir);

  // 3、关闭目录
  closedir(dirp);

  return 0;
}
