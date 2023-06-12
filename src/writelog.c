#include "writelog.h"

_logset loglevel = LOG_ERROR;

void _writelog(const char *level, const char *filename, const int line, const char *funcname, const char * args, ...)
{
    char time_string[128] = {0,};
    va_list va;

    _getnow(time_string);
    printf("%-20s [%s] %s (%s:%d) : ", time_string, level, filename, funcname, line);

    va_start(va, args);
    vprintf(args, va);
    va_end(va);
}

void _getnow(char *buf)
{
    time_t now = time(NULL);
    strftime(buf, sizeof(char) * 128, "%Y-%m-%d %H:%M:%S", localtime(&now));
}

void init_wlog()
{
    loglevel = LOG_DEBUG;
}

bool createlogfile(char *dir)
{
    FILE *file;

    bool ret = false;
    char dirbuf[256] = {0,};

    if(dir != NULL)
    {
        snprintf(dirbuf, sizeof(dirbuf), "%s", dir);
        file = fopen(dirbuf, "a+");
    }
    else
    {
        return ret;
    }

    if(file != NULL)
    {
        ret = true;
        fclose(file);             // 해당 파일에 로그 파일 쓰고 프로세스 종료가 될 때 닫자
    }

    return ret;
}