#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char *argv[])
{
    int i = 0;

    if (argc < 2)
    {
        printf("users : %s < ÉèÖÃ×ÜÊý >\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    for(i = 0; i < atoi(argv[1]); i++)
    {
        if(i >= 100)
        {
            printf("%d", i);
        }
        else if(i >= 10)
        {
            printf("0%d", i);
        }
        else
        {
            printf("00%d", i);
        }
    }

    while(1);

    return 0;
}
