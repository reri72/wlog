#include "writelog.h"

_logset loglevel = LOG_ERROR;

void init_wlog(_logset set)
{
    memset(&li, 0x00, sizeof(_loginfo_t));
    loglevel = set;
}

void _writelog(const char *level, const char *filename, const int line, const char *funcname, const char * args, ...)
{
    char time_string[128] = {0,};
    char logbuffer[MAX_SIZE] = {0,};
    char argbuf[MAX_SIZE] = {0,};
    va_list va;

    va_start(va, args);
    vsprintf(argbuf, args, va);
    va_end(va);

    _getnow(time_string);
    snprintf(logbuffer, MAX_SIZE, "%-20s [%s] %s  %s(%d) :  %s", time_string, level, funcname, filename, line, argbuf);

    printf("%s", logbuffer);    // continue...
}

void _getnow(char *buf)
{
    time_t now = time(NULL);
    strftime(buf, sizeof(char) * 128, "%Y-%m-%d %H:%M:%S", localtime(&now));
}

bool createlogfile(char *dir, char *name)
{
    FILE *file;
    DIR *dirinfo;
    struct dirent *direntry;
    bool ret = false;

    if(dir != NULL)
    {
        mkdir(dir, 0755);

        dirinfo = opendir(dir);
        if (dirinfo != NULL)
        {
            strcpy(li.dir, dir);
#if 0
            while (direntry = readdir(dirinfo))
            {
                printf("%s \n",direntry->d_name);
            }
#endif
            file = fopen(name, "a+");
            if(file != NULL)
            {
                ret = true;
                strcpy(li.fname, name);
                fclose(file);
            }
            closedir(dirinfo);
        }
    }
 
    return ret;
}