#include <stdio.h>
#include <time.h>

#include "wutil.h"

void get_now(char *buf)
{
    time_t now = time(NULL);
    struct tm ts;
    localtime_r(&now, &ts);

    strftime(buf, sizeof(char) * 128, "%04d-%02d-%02d %02d:%02d:%02d", &ts);
}

void n_sleep(int sec, int nsec)
{
    struct timespec rem, req;

    req.tv_sec = sec;
    req.tv_nsec = nsec;

    nanosleep(&req, &rem);

    return;
}