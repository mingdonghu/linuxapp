#include "./hero.h"

int main(int argc, const char *argv[])
{
    pthread_t luban_thread;
    pthread_t houyi_thread;

    if (pthread_create(&luban_thread, NULL, luban_thread_handler, NULL) != 0)
    {
        perror("pthread_create luban");
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&houyi_thread, NULL, houyi_thread_handler, NULL) != 0)
    {
        perror("pthread_create houyi");
        exit(EXIT_FAILURE);
    }

    if(pthread_join(luban_thread, NULL) == 0)
    {
        printf("luban join\n");
    }
    else
    {
        perror("luban join fail");
        exit(EXIT_FAILURE);
    }
    
    if(pthread_join(houyi_thread, NULL) == 0)
    {
        printf("houyi join\n");
    }
    else
    {
        perror("houyi join fail");
        exit(EXIT_FAILURE);
    }

    return 0;
}
