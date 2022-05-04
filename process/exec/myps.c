#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, const char *argv[])
{
    int ret_execlp;

    ret_execlp = execlp("ps", "ps", "-ef", NULL);
    if(ret_execlp < 0)
    {
        perror("execlp .");
        exit(EXIT_FAILURE);
    }

    return 0;
}
