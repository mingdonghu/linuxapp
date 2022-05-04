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
    ssize_t ret_write;
    char wbuffer[] = "hello hqyj";

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
    fifo_fd = open("./FIFO", O_WRONLY);
    if(fifo_fd == -1)
    {
        perror("open()");
        exit(EXIT_FAILURE);
    }

    //向管道中写数据
    //ssize_t write(int fd, const void *buf, size_t count);
    ret_write = write(fifo_fd, wbuffer, 10);
    if(ret_write == -1)
    {
        perror("write()");
        exit(EXIT_FAILURE);
    }
    printf("write to fifo\n");

    //关闭管道
    close(fifo_fd);

    return 0;
}
