#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define BUFFERSIZE 10

int main(int argc, const char *argv[])
{
    FILE *fp_src;
    FILE *fp_dest;
    char rbuffer[BUFFERSIZE];
    int line = 0;
    char c;
    int size_read = 0;
    
    //使用命令行参数
    if(argc < 3)
    {
        fprintf(stdout, "users : %s < 源文件 > < 目标文件 >\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    //FILE *fopen(const char *path, const char *mode);
    if ((fp_src = fopen(argv[1], "r+")) == NULL)
    {
        fprintf(stderr, "open %s failed : %s\n", argv[1], strerror(errno));
        exit(EXIT_FAILURE);
    }

    //FILE *fopen(const char *path, const char *mode);
    if ((fp_dest = fopen(argv[2], "w+")) == NULL)
    {
        fprintf(stderr, "open %s failed : %s\n", argv[2], strerror(errno));
        fclose(fp_src);
        exit(EXIT_FAILURE);
    }

#if 0
    printf("line mode\n");
    while(fgets(rbuffer, BUFFERSIZE, fp_src) != NULL)
    {
        if (strlen(rbuffer) < BUFFERSIZE - 1 || rbuffer[BUFFERSIZE - 2] == '\n')
        {
            line++;
        }
        fputs(rbuffer, fp_dest);
    }
#endif

#if 1
    printf("block mode\n");
    while((size_read = fread(rbuffer, 1, 10, fp_src)) == 10)
    {
        fwrite(rbuffer, 1, size_read, fp_dest);
    }
    fwrite(rbuffer, 1, size_read, fp_dest);
#endif

#if 0
    printf("char mode\n");
    while((c = fgetc(fp_src)) != EOF)
    {
        fputc(c, fp_dest);
    }
#endif

    printf("line : %d\n", line);

    fclose(fp_src);
    fclose(fp_dest);

    return 0;
}
