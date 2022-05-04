#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

int g_num = 0;

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

void *pthread_add(void *arg)
{
    int loops = *((int*)arg);
    int loc, j;
    for(j = 0; j < loops; j++)
    {
        pthread_mutex_lock(&mtx);
        loc = g_num;
        loc++;
        g_num = loc;
        pthread_mutex_unlock(&mtx);
    }
    pthread_exit(NULL);
}

int main(int argc, const char *argv[])
{
    pthread_t tid[2];
    int no = 0;
    int count = 0;
    int err_create, err_jion;

    if(argc < 2)
    {
        printf("users : %s < 计算次数 >\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    count = atoi(argv[1]);

    for (no = 0; no < 2; no++)
    {
        err_create = pthread_create(&tid[no], NULL, pthread_add, &count);
        if(err_create != 0)
        {
            printf("pthread_create() : %s\n", strerror(err_create));
            exit(EXIT_FAILURE);
        }

    }

    for (no = 0; no < 2; no++)
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

    printf("g_num = %d\n", g_num);
    
    return 0;
}
