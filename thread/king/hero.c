#include "./hero.h"

void *luban_thread_handler(void *arg)
{
    int n = 5;
    while(n--)
    {
        sleep(1);
        printf("luban dadada\n");
    }
    pthread_exit(NULL);
}

void *houyi_thread_handler(void *arg)
{
    int n = 5;
    while(n--)
    {
        sleep(2);
        printf("houyi biubiubiu\n");
    }
    pthread_exit(NULL);
}

