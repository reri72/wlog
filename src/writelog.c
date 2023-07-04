#include "writelog.h"

_logset loglevel = LOG_ERROR;

void *log_thread(void *arg)
{
    while(status)
    {
        //n_sleep(0, 50);
        n_sleep(1, 0);

        if(get_list_length() > 0)
        {
            if(li.lfile != NULL)
            {
                int ret = 1;
                while(ret)
                {
                    ret = _writetext();
                }

                //if( _szchk() > (KBYTE * KBYTE * KBYTE) )
                if( _szchk() > 1024 )
                {
                    bool res = _lotate_file();
                    if(res)
                    {
                        printf("lotation success!! \n");
                    }
                }
            }
            printf("len : %d \n", get_list_length());
        }
        else
        {
            continue;
        }
    }

    n_sleep(0, 10);

    return NULL;
}

void _init_wlog(_logset set)
{
    status = true;
    
    loglist = malloc(sizeof(loglist));
    loglist->next = NULL;

    memset(&li, 0x00, sizeof(_loginfo_t));
    loglevel = set;

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
}

void _destroy_wlog()
{
    pthread_mutex_destroy(&mutex);

    llist_t *cur = loglist;
    llist_t *next = NULL;

    status = false;
    n_sleep(0, 100);

    while(cur != NULL)
    {
        next = cur->next;
        free(cur);
        cur = next;
    }

    if(li.lfile != NULL)
    {
        fclose(li.lfile);
    }
}

void _changellevel(_logset set)
{
    loglevel = set;
}

void _insertlog(const char *level, const char *filename, const int line, const char *funcname, const char * args, ...)
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

    add_list_front(logbuffer);
}

void _getnow(char *buf)
{
    time_t now = time(NULL);
    strftime(buf, sizeof(char) * 128, "%Y-%m-%d %H:%M:%S", localtime(&now));
}

int _szchk()
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

bool _create_log(char *dir, char *name)
{
    bool ret = false;

    DIR *dirinfo;
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
            li.lfile = fopen(name, "a+");
            if(li.lfile != NULL)
            {
                ret = true;
                strcpy(li.fname, name);
                snprintf(li.fullpath, 641, "%s%s", li.dir, li.fname);
            }
            else
            {
                ret = false;
                status = false;
            }
            closedir(dirinfo);
        }
        else
        {
            ret = false;
            status = false;
        }
    }
 
    return ret;
}

bool _lotate_file()
{
    char nname[128] = {0,};
    char nname2[128] = {0,};
    char nfname[128] = {0,};

    bool ret = false;
    int i;
    
    if(li.lfile != NULL)
    {
        fclose(li.lfile);
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
    
    ret = _create_log(li.dir, li.fname);
    if(!ret)
    {
        printf("lotation is failed \n");
    }

    return ret;
}

int _writetext()
{
    int ret = 1;

    if(li.lfile != NULL)
    {
        llist_t *cur = loglist;
        llist_t *next = NULL;
        
        ret = pthread_mutex_trylock(&mutex);
        if(ret == 0)
        {
            while(cur != NULL)
            {
                next = cur->next;
                fprintf(li.lfile, cur->text);
                cur = next;
            }
            free(cur);
            pthread_mutex_unlock(&mutex);
        }
    }
    else
    {
        return 1;
    }

    return 0;
}

int get_list_length()
{
    int len = -1;
    llist_t *cur = loglist;

    while(cur != NULL)
    {
        len++;
        cur = cur->next;
    }

    return len; 
}

void add_list_front(char* nettext)
{
    llist_t *cur = loglist;
    llist_t *newitem = malloc(sizeof(llist_t));
    
    newitem->next = cur->next;
    newitem->text = nettext;

    cur->next = newitem;
}

void n_sleep(int sec, int nsec)
{
    struct timespec rem, req;

    req.tv_sec = sec;
    req.tv_nsec = nsec;

    nanosleep(&req, &rem);

    return;
}
