#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

//线程锁初始化
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

//共享变量
int g_num = 0;

void *thread_produce(void *arg)
{
    int count = atoi((char *)arg);
    int i;

    for (i = 0; i < count; i++)
    {
        pthread_mutex_lock(&mtx);//上锁

        printf("线程 [%d] 生产了一个产品，产品数量为：%d\n", (int )pthread_self(), ++g_num);

        pthread_mutex_unlock(&mtx);//开锁

        sleep(1);
    }

    pthread_exit(NULL);
}

int main(int argc, const char *argv[])
{
    pthread_t tid;
    int err_create;
    int total_prodoce = 0; //总生产数量
    int total_consume = 0; //总消费数量
    int i;
    bool done = false;

    for (i = 0; i < argc; i++)
    {
        total_prodoce += atoi(argv[i]);
        err_create = pthread_create(&tid, NULL, thread_produce, (void *)argv[i]);
        if(err_create != 0)
        {
            perror("pthread_create()");
            exit(EXIT_FAILURE);
        }
    }

    while (1)
    {
        pthread_mutex_lock(&mtx);//上锁

        while(g_num > 0)
        {
            total_consume++;
            printf("消费一个产品，产品数量为：%d\n", --g_num);
            done = total_consume >= total_prodoce;
            sleep(1);
        }

        pthread_mutex_unlock(&mtx);//开锁

        if(done)
        {
            break;
        }
    }

    return 0;
}
