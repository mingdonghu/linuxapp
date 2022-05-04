#include <stdio.h>

/*
 * alarm 实现 mysleep
 */

void mysleep(int seconds)
{
    alarm(seconds);
    pause();
}

int main(int argc, const char *argv[])
{
    printf("hello hqyj 1\n");
    mysleep(3);

    printf("hello hqyj\n");

    return 0;
}
