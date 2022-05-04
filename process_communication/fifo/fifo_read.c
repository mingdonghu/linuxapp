#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/*
 * 向管道中写数据
 */

int main(int argc, const char *argv[])
{
    int ret_mkfifo;
    int fifo_fd;
    ssize_t ret_read;
    char rbuffer[10];

    //如果没有管道创建管道
    //int access(const char *pathname, int mode);
    if(access("./FIFO", F_OK) != 0)
    {
        ret_mkfifo = mkfifo("./FIFO", 0666);
        if(ret_mkfifo == -1)
        {
            perror("mkfifo()");
            exit(EXIT_FAILURE);
        }
    }

    //打开管道
    fifo_fd = open("./FIFO", O_RDONLY);
    if(fifo_fd == -1)
    {
        perror("open()");
        exit(EXIT_FAILURE);
    }

    //从管道中读数据
    //ssize_t read(int fd, const void *buf, size_t count);
    ret_read = read(fifo_fd, rbuffer, 10);
    if(ret_read == -1)
    {
        perror("read()");
        exit(EXIT_FAILURE);
    }
    printf("read to fifo\n");

    //关闭管道
    close(fifo_fd);

    return 0;
}
