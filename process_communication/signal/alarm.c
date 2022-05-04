#include <stdio.h>
#include <unistd.h>

/*
 * alarm 闹钟
 */

int main(int argc, const char *argv[])
{
    unsigned int ret_alarm;

    printf("alarm 3\n");
    ret_alarm = alarm(3);
    printf("ret_alarm = %d\n", ret_alarm);

    printf("sleep 2\n");
    sleep(2);

    ret_alarm = alarm(3);
    printf("ret_alarm = %d\n", ret_alarm);

    while(1)
    {
        sleep(1);
        printf("hello hqyj\n");
    }

    return 0;
}
