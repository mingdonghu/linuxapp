#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, const char *argv[])
{
    int ret_stat;

    struct stat statBuffer;
    ret_stat = stat(".", &statBuffer);
    printf("mode\tnlink\tuid\tgid\tsize\tctime\n");
    fprintf(stdout, "%d\t", statBuffer.st_mode);

    return 0;
}
