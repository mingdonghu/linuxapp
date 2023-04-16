/**
 * 从一个文件(源文件)中读取最后10KB数据并复制到另一个文件(目标文件)
 * 
*/

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define BUFFER_SIZE      1024              // 规定读写缓存区的大小，char [BUFFER_SIZE] 数组的大小，
                                           // 因为其数组的每个单元为char 型，故其单位为字节，因此确定了1KB的缓冲区
#define SRC_FILE_NAME    "src_file.txt"   // 源文件名
#define DEST_FILE_NAME   "dest_file.txt" // 目标文件名
#define OFFSET           (10*1024)       // 复制的数据大小

int main(int argc, char const *argv[]) {
  unsigned char buff[BUFFER_SIZE];  // 定义读写缓存区
  
  // 1. 打开源文件 只读方式
  int fd_src_file;
  fd_src_file = open(SRC_FILE_NAME, O_RDONLY);
  if (fd_src_file < 0) {
    perror("fail to open src_file.");
    exit(EXIT_FAILURE);
  }
  
  // 2. 打开目标文件，以只写方式
  int fd_dest_file;
  int open_flag = (O_WRONLY | O_CREAT | O_TRUNC);
  fd_dest_file = open(DEST_FILE_NAME, open_flag, 0644);
  if (fd_dest_file < 0) {
    perror("fail to open dest_file.");
    exit(EXIT_FAILURE);
  }

  // 3. 将源文件的读写指针移到最后的10KB的起始位置
  lseek(fd_src_file, -OFFSET, SEEK_END);

  // 将源文件的最后10KB数据，然后写入到目标文件中，每次读1KB
  int read_len;
  while (1) {
    read_len = read(fd_src_file, buff, sizeof(buff)/sizeof(buff[0]));
    if (read_len > 0) {
      // write data to dest file.
      int write_len = write(fd_dest_file, buff, BUFFER_SIZE);
      if (write_len < 0) {
        perror("fail to write dest_fail.");
        exit(EXIT_FAILURE);
      } else {
        printf("write len: %d\n", write_len);
      }
    } else {
      if (read_len < 0) {
        perror("fail to read src_file.");
        exit(EXIT_FAILURE);
      } else {
        printf("read the last 10KB of the src file.\n");
        break;
      }
    }
  }

  if (close(fd_src_file) == -1) {
    perror("fail to close src_file.");
    exit(EXIT_FAILURE);
  }

  if (close(fd_dest_file) == -1) {
    perror("fail to close dest_file.");
    exit(EXIT_FAILURE);
  }

  return 0;
}

