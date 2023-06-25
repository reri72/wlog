#include "writelog.h"

_logset loglevel = LOG_ERROR;

void _init_wlog(_logset set)
{
    pthread_mutex_init(&mutex, NULL);

    memset(&li, 0x00, sizeof(_loginfo_t));
    loglevel = set;
}

void _changellevel(_logset set)
{
    loglevel = set;
}

void _writelog(const char *level, const char *filename, const int line, const char *funcname, const char * args, ...)
{
    char time_string[128] = {0,};
    char logbuffer[MAX_ASIZE] = {0,};
    char argbuf[MAX_ASIZE] = {0,};
    va_list va;

    va_start(va, args);
    vsprintf(argbuf, args, va);
    va_end(va);

    _getnow(time_string);
    snprintf(logbuffer, MAX_SIZE, "%-20s [%s]  %s  %s(%d) :  %s",
                                    time_string, level, funcname, filename, line, argbuf);

    int ret = 1;
    while(ret)
    {
        ret = pthread_mutex_trylock(&mutex);
        if(ret == 0)
        {
            ret = _writetext(logbuffer);
            pthread_mutex_unlock(&mutex);
        }
    }
}

void _getnow(char *buf)
{
    time_t now = time(NULL);
    strftime(buf, sizeof(char) * 128, "%Y-%m-%d %H:%M:%S", localtime(&now));
}

bool _create_log(char *dir, char *name)
{
    bool ret = false;

    DIR *dirinfo;
    FILE *lfile;
    struct dirent *direntry;

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
            lfile = fopen(name, "a+");
            if(lfile != NULL)
            {
                ret = true;
                strcpy(li.fname, name);
                fclose(lfile);

                snprintf(li.fullpath, 641, "%s%s", li.dir, li.fname);
            }
            else
            {
                ret = false;
            }
            closedir(dirinfo);
        }
        else
        {
            ret = false;
        }
    }
 
    return ret;
}

int _writetext(char *text)
{
    FILE *lfile = NULL;

    lfile = fopen(li.fullpath, "a+");
    if(lfile != NULL)
    {
        fprintf(lfile, text);
        fclose(lfile);
    }
    else
    {
        return 1;
    }

    return 0;
}