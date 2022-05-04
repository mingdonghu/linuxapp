/*
 * 功能：
 * 获取系统时间，每秒在终端打印一下
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, const char *argv[])
{
    time_t t;
    struct tm *mytm;

    //每秒打印时间
    while(1)
    {
        t = time(NULL);
        mytm = localtime(&t);

        printf("year\tmonth\tday\twday\thour\tmin\tseconds\tAM/PM\n");
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
            mytm->tm_year + 1900,
            mytm->tm_mon + 1,
            mytm->tm_mday,
            mytm->tm_wday,
            mytm->tm_hour + 3,
            mytm->tm_min,
            mytm->tm_sec,
            mytm->tm_isdst);

        sleep(1);
    }


    return 0;
}
