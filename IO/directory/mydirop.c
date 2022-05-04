#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

void exam()
{
    DIR *dir;
    struct dirent *pdir;
    int dir_count = 0;

    dir = opendir("./");

    while(1)
    {
        pdir = readdir(dir);
        if(pdir == NULL)
        {
            break;
        }

        if(pdir->d_name[0] == '.')
        {
            continue;
        }

        if(pdir->d_type == DT_REG)
        {
            dir_count ++;   
        }
    }

    printf("dir count: %d\n", dir_count);

    return;
}

int main(int argc, const char *argv[])
{
    DIR *dir;

    if(argc < 2)
    {
        printf("please input path\n");
        exit(EXIT_FAILURE);
    }



    //打开目录
    //DIR *opendir(const char *name);
    dir = opendir(argv[1]);

    //读目录里的内容
    //struct dirent *readdir(DIR *dirp);
    struct dirent *pdir;

#if 0
    pdir = readdir(dir);

    printf("name: %s\n", pdir->d_name);

    pdir = readdir(dir);

    printf("name: %s\n", pdir->d_name);

    pdir = readdir(dir);

    printf("name: %s\n", pdir->d_name);
#endif

    int count = 0;

    while(1)
    {
        pdir = readdir(dir);
        if(pdir == NULL)
        {
            break;
        }

            count ++;
    }

    printf("count: %d\n", count);

    exam();

    return 0;
}
