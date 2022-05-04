/*
 * function:
 * copy file.txt to file_copy.txt
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, const char *argv[])
{
    int fd_src;

    int fd_dest;

    char rbuffer[10];

    //user command
    if(argc < 3)
    {
        printf("users : %s < dest file > < src file >\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    //open src file
    fd_file = open(argv[2], O_RDONLY);
    if(fd_file == -1)
    {
        perror("open file");
        exit(EXIT_FAILURE);
    }
    //open dest file
    

    read(fd_src, rbuffer, 10);

    write(fd_dest, rbuffer, 10);

    printf("%s\n", rbuffer);
    //1、open file.txt, open file_copy.txt

    //2、read file.txt , write wo file_copy.txt
    
    //3、close all

    return 0;
}
