#include "writelog.h"

_logset loglevel = LOG_DEBUG;

void _writelog(const char *filename, const int line, const char *funcname, const char * args, ...)
{
    char time_string[128] = {0,};
    _getnow(time_string);

    va_list va;
    va_start(va, args);
    
    printf("%-20s %s %s %d : ", time_string, filename, funcname, line);
    vprintf(args, va);

    va_end(va);
}

void _getnow(char *buf)
{
    time_t now = time(NULL);
    strftime(buf, sizeof(char) * 128, "%Y-%m-%d %H:%M:%S", localtime(&now));
}