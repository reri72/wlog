#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pthread.h>

#include "writelog.h"
#include "wutil.h"

_logset     loglevel = LOG_ERROR;
logq_t      logqueue;
_loginfo_t  li;

pthread_t tid;
pthread_mutex_t _mutex;

volatile bool status = false;

void *log_thread(void *arg)
{
    while(status)
    {
        //n_sleep(0, 50);
        n_sleep(1, 0);

        if(get_lque_size(&logqueue) > 0)
        {
            if(li.lfile)
            {
                fwrite_text();

                if( logfile_size_check() > (KBYTE) )
                {
                    int res = lotate_file();
                    if (res < 0)
                    {
                        printf("log lotation failed. \n");
                        fprintf(stderr, "log lotation failed\n");
                    }
                }
            }
        }
        else
        {
            continue;
        }
    }
    n_sleep(0, 10);

    return NULL;
}


int init_wlog(_logset set, int max)
{
    status = true;

    loglevel = set;
    
    logqueue.num = 0;
    logqueue.max = max;

    memset(&li, 0x00, sizeof(_loginfo_t));

    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);  // re-entry

    pthread_mutex_init(&_mutex, &attr);
    pthread_mutexattr_destroy(&attr);

    if( pthread_create(&tid, NULL, log_thread, NULL) != 0 )
    {
        printf("[%s] pthread_createe failed. ", __FUNCTION__);
        fprintf(stderr, "pthread_createe failed\n");
        perror("thread create error");
        exit(0);
    }

    if( pthread_detach(tid) != 0 )
    {
        printf("[%s] pthread_detach failed. ", __FUNCTION__);
        fprintf(stderr, "pthread_detach failed\n");
        perror("thread detach error");
        exit(0);
    }

    return 0;
}


void terminate_wlog(logq_t *que)
{
    clear_lque(que);
    que->max = que->num = 0;
}


void destroy_wlog()
{
    pthread_mutex_destroy(&_mutex);

    status = false;
    n_sleep(0, 100);

    terminate_wlog(&logqueue);

    close(li.lfile);
}


void _changellevel(_logset set)
{
    loglevel = set;
}


void _insert(const char *level, const char *filename, const int line, const char *funcname, const char * args, ...)
{
    char time_string[128] = {0,};
    char logbuffer[2048] = {0,};
    char argbuf[2048] = {0,};
    va_list va;

    va_start(va, args);
    vsprintf(argbuf, args, va);
    va_end(va);

    get_now(time_string);
    snprintf(logbuffer, MAX_SIZE, "%-20s [%s]  %s  %s(%d) :  %s",
                                    time_string, level, funcname, filename, line, argbuf);

    pthread_mutex_lock(&_mutex);
    {
        add_logtext(logbuffer);
    }
    pthread_mutex_unlock(&_mutex);
}


int create_logfile(char *dir, char *name)
{
    int ret = -1;
    DIR *dirinfo;
    dirinfo = opendir(dir);

    if (dirinfo == NULL)
    {
        mkdir(dir, 0755);
    }

    {
        if(strlen(li.dir) < 1)
            strcpy(li.dir, dir);
        if(strlen(li.fname) < 1)
            strcpy(li.fname, name);

        if(strlen(li.fullpath) < 1)
            snprintf(li.fullpath, 1024, "%s%s", li.dir, li.fname);

        li.lfile = open(li.fullpath, O_WRONLY|O_CREAT|O_EXCL, 0644);
        if(li.lfile < 0)
            li.lfile = open(li.fullpath, O_WRONLY);

        if(li.lfile > 0)
            ret = 0;

        closedir(dirinfo);
    }
 
    return ret;
}


int lotate_file()
{
    char nname[1027] = {0,};
    char nname2[1027] = {0,};
    char nfname[1027] = {0,};

    int i;
    
    close(li.lfile);

    for(i = 9; i > 0; i--)
    {
        snprintf(nname, 1026, "%s%s.%d", li.dir, li.fname, i);
        snprintf(nname2, 1026, "%s%s.%d", li.dir, li.fname, (i+1));
        rename(nname, nname2);

        nname2[0] = 0;
        nname[0] = 0;
    }

    snprintf(nfname, 1026, "%s.%d", li.fullpath, 1);
    rename(li.fullpath, nfname);

    int ret = create_logfile(li.dir, li.fname);
    if (ret < 0)
    {
        return -1;
    }

    return 0;
}


int fwrite_text()
{
    int ret = W_TRUE;

    if(li.lfile)
    {
        pthread_mutex_lock(&_mutex);
        {
            if(logqueue.num > 0)
            {
                int i;
                for(i = 0; i < logqueue.num; i++)
                {
                    write(li.lfile, (const char*)logqueue.text[i], strlen(logqueue.text[i]));
                }
                clear_lque(&logqueue);
            }
        }
        pthread_mutex_unlock(&_mutex);
    }
    else
    {
        ret = W_FALSE;
    }

    return ret;
}


int logfile_size_check()
{
    struct stat finfo;
    
    if(stat(li.fullpath, &finfo) < 0)
    {
        printf("[%s] log file size check error. \n", __FUNCTION__);
        perror("stat");
        exit(1);
    }

    return finfo.st_size;           //byte
}


int get_lque_size(const logq_t *que)
{
    return que->num;
}


void print_lque(const logq_t *que)
{
    if(que->num > 0)
    {
        int i;
        for(i = 0; i < que->num; i++)
        {
            printf("queue[%d] : %s \n", i, que->text[i]);
        }
    }
}


void add_logtext(char* newtext)
{
    pthread_mutex_lock(&_mutex);
    {
        logqueue.text[logqueue.num] = (char*)malloc( strlen(newtext) + 1 );
        if (logqueue.text[logqueue.num] != NULL)
        {
            strcpy(logqueue.text[logqueue.num++], newtext);
        }
        else
        {
            fprintf(stderr, "memory allocation failure \n");
        }
    }
    pthread_mutex_unlock(&_mutex);
}


void clear_lque(logq_t *que)
{
    if (que->num > 0)
    {
        for (int i = 0; i < que->num; i++)
        {
            if(que->text[i])
            {
                free(que->text[i]);
                que->text[i] = NULL;
            }
        }
        que->num = 0;
    }
}

