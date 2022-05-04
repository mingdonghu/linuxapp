#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

int g_num = 0;

void *do_pthread_handler(void *arg)
{
    int thrd_num = *((int *)arg);
    int delay_time = 0;
    int count = 0;

    printf("Thread %d is starting\n", thrd_num);
    for (count = 0; count < 5; count++)
    {
        delay_time = (int )(rand() * 6.0 / (RAND_MAX)) + 1;
        sleep(delay_time);
        g_num++;
        printf("\tThread %d : job %d delay = %d g_num = %d\n", thrd_num, count, delay_time, g_num);
    }

    printf("Thread %d finished\n", thrd_num);

    pthread_exit(NULL);
}

int main(int argc, const char *argv[])
{
    pthread_t tid[3];
    int no = 0;
    int err_create, err_jion;

    srand(time(NULL));

    for (no = 0; no < 3; no++)
    {
        err_create = pthread_create(&tid[no], NULL, do_pthread_handler, (void *)&no);
        if(err_create != 0)
        {
            printf("pthread_create() : %s\n", strerror(err_create));
            exit(EXIT_FAILURE);
        }

    }

    printf("create threads success\n wait for threads to finish...\n");

    for (no = 0; no < 3; no++)
    {
        err_jion = pthread_join(tid[no], NULL);
        if(!err_jion)
        {
            printf("Thread %d joined\n", no);
        }
        else
        {
            printf("Thread %d join failed\n", no);
        }
    }
    
    return 0;
}
