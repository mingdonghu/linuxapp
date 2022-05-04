/*
 * 功能：
 * open file and write "hello hqyj!"
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(int argc, const char *argv[])
{
    int fd_file;
    ssize_t wnum;
    char wbuffer[] = "hello hqyj!";

    //1、open file
    fd_file = open("./file.txt", O_WRONLY);
    if(fd_file == -1)
    {
        perror("open file.txt");
        exit(EXIT_FAILURE);
    }

    //2、write "hello hqyj!"
    wnum = write(fd_file, wbuffer, 10);
    if(wnum == -1)
    {
        printf("write failure : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    //3、close file
    if(close(fd_file) == -1)
    {
        perror("close file.txt");
        exit(EXIT_FAILURE);
    }

    return 0;
}
