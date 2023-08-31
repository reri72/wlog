#include "writelog.h"

_logset loglevel = LOG_ERROR;

void *log_thread(void *arg)
{
    int i = 0;
    while(status)
    {
        //n_sleep(0, 50);
        n_sleep(1, 0);

        if(get_lque_size(&logqueue) > 0)
        {
            if(li.lfile != NULL)
            {
                fwrite_text();

                if( logfile_size_check() > (KBYTE) )
                {
                    bool res = lotate_file();
                    if(!res)
                    {
                        printf("lotation failed. \n");
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
    
    pthread_mutex_init(&mutex, NULL);
    
    if( pthread_create(&tid, NULL, log_thread, NULL) != 0 )
    {
        printf("[%s] pthread_createe failed. ", __FUNCTION__);
        perror("thread create error");
        exit(0);
    }

    if( pthread_detach(tid) != 0 )
    {
        printf("[%s] pthread_detach failed. ", __FUNCTION__);
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
    pthread_mutex_destroy(&mutex);

    status = false;
    n_sleep(0, 100);

    terminate_wlog(&logqueue);

    if(li.lfile != NULL)
    {
        fclose(li.lfile);
    }
}

void _changellevel(_logset set)
{
    loglevel = set;
}

void _insert(const char *level, const char *filename, const int line, const char *funcname, const char * args, ...)
{
    char time_string[128] = {0,};
    char logbuffer[MAX_ASIZE] = {0,};
    char argbuf[MAX_ASIZE] = {0,};
    int ret = 1;
    va_list va;

    va_start(va, args);
    vsprintf(argbuf, args, va);
    va_end(va);

    get_now(time_string);
    snprintf(logbuffer, MAX_SIZE, "%-20s [%s]  %s  %s(%d) :  %s",
                                    time_string, level, funcname, filename, line, argbuf);

    if(pthread_mutex_trylock(&mutex) == 0)
    {
        add_logtext(logbuffer);
        pthread_mutex_unlock(&mutex);
    }
}

void get_now(char *buf)
{
    time_t now = time(NULL);
    strftime(buf, sizeof(char) * 128, "%Y-%m-%d %H:%M:%S", localtime(&now));
}

bool create_logfile(char *dir, char *name)
{
    bool ret = false;

    DIR *dirinfo;
    struct dirent *direntry;

    if(dir != NULL)
    {
        mkdir(dir, 0755);
    }

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

        li.lfile = fopen(name, "a+");
        if(li.lfile != NULL)
        {
            ret = true;
            strcpy(li.fname, name);
            snprintf(li.fullpath, 641, "%s%s", li.dir, li.fname);
        }
        closedir(dirinfo);
    }
 
    return ret;
}

bool lotate_file()
{
    char nname[128] = {0,};
    char nname2[128] = {0,};
    char nfname[128] = {0,};

    bool ret = false;
    int i;
    
    if(fclose(li.lfile) != 0)
    {
        printf("fclose failed. \n");
    }

    for(i = 9; i > 0; i--)
    {
        snprintf(nname, 641, "%s%s.%d", li.dir, li.fname, i);
        snprintf(nname2, 641, "%s%s.%d", li.dir, li.fname, i+1);
        rename(nname, nname2);

        nname2[0] = 0;
        nname[0] = 0;
    }

    snprintf(nfname, 641, "%s.%d", li.fullpath, 1);
    rename(li.fullpath, nfname);

    ret = create_logfile(li.dir, li.fname);
    if(!ret)
    {
        printf("lotation is failed \n");
    }

    return ret;
}

int fwrite_text()
{
    int ret = 0;

    if(li.lfile != NULL)
    {
        if(pthread_mutex_trylock(&mutex) == 0)
        {
            if(logqueue.num > 0)
            {
                int i = 0;
                for(i; i < logqueue.num; i++)
                {
                    fprintf(li.lfile, (const char*)logqueue.text[i]);
                }
                clear_lque(&logqueue);
            }
            pthread_mutex_unlock(&mutex);
        }
    }
    else
    {
        ret = -1;
    }

    return ret;
}

int logfile_size_check()
{
    struct stat finfo;
    int res = 0;
    
    if(stat(li.fullpath, &finfo) < 0)
    {
        printf("[%s] file size check error. \n", __FUNCTION__);
        perror("stat");
        exit(1);
    }

    return finfo.st_size;           //byte
}

int get_lque_size(logq_t *que)
{
    return que->num;
}

void print_lque(const logq_t *que)
{
    if(que->num > 0)
    {
        int i = 0;
        for(i; i < que->num; i++)
        {
            printf("queue[%d] : %s \n", i, que->text[i]);
        }
    }
}

void add_logtext(char* newtext)
{
    logqueue.text[logqueue.num] = (char*)malloc( strlen(newtext) + 1 );
    strcpy(logqueue.text[logqueue.num++], newtext);
}

void clear_lque(logq_t *que)
{
    if(que->num > 0)
    {
        int i = 0;
        int j = que->num;
        for(i; i < j; i++)
        {
            if(que->text[i])
            {
                free(que->text[i]);
                que->num--;
            }
        }
    }
}

void n_sleep(int sec, int nsec)
{
    struct timespec rem, req;

    req.tv_sec = sec;
    req.tv_nsec = nsec;

    nanosleep(&req, &rem);

    return;
}