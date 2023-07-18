#include "writelog.h"

_logset loglevel = LOG_ERROR;

void *log_thread(void *arg)
{
    while(status)
    {
        //n_sleep(0, 50);
        n_sleep(1, 0);

        if(get_list_length(loglist) > 0)
        {
            if(li.lfile != NULL)
            {
                _writetext(loglist);

                if( _szchk() > (KBYTE * KBYTE * KBYTE) )
                {
                    bool res = _lotate_file();
                    if(res)
                    {
                        printf("lotation success!! \n");
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

void _init_wlog(_logset set)
{
    status = true;
    
    loglist = (llist_t *)malloc(sizeof(llist_t));
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

    status = false;
    n_sleep(0, 100);

    del_all_node(loglist);

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
    int ret = 1;
    va_list va;

    va_start(va, args);
    vsprintf(argbuf, args, va);
    va_end(va);

    _getnow(time_string);
    snprintf(logbuffer, MAX_SIZE, "%-20s [%s]  %s  %s(%d) :  %s",
                                    time_string, level, funcname, filename, line, argbuf);

    if(pthread_mutex_trylock(&mutex) == 0)
    {
        printf("%s \n", logbuffer);
        add_list_item(loglist, logbuffer);
        pthread_mutex_unlock(&mutex);
    }

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

int _writetext(llist_t *list)
{
    int ret = 1;

    if(li.lfile != NULL)
    {
        if(pthread_mutex_trylock(&mutex) == 0)
        {
            llist_t *cur = list->next;
            while(cur != NULL)
            {
                fprintf(li.lfile, (const char*)cur->text);
                //printf("%s ] %s \b", __FUNCTION__, cur->text);
                cur = cur->next;
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

int get_list_length(llist_t *list)
{
    int len = 0;
    llist_t *cur = list;

    while(cur != NULL)
    {
        len++;
        cur = cur->next;
    }
    return len;
}

void add_list_item(llist_t *list, char* newtext)
{
    llist_t *cur = list;
    llist_t *newnode = malloc(sizeof(llist_t));

    printf("%s ] %s \b", __FUNCTION__, newtext);
    
    newnode->next = NULL;
    newnode->text = newtext;

    if(cur == NULL)
    {
        cur->next = newnode;
    }
    else
    {
        while (cur->next != NULL)
        {
            cur = cur->next;
        }
        cur->next = newnode;
    }
}

void print_list(llist_t *list)
{
    llist_t *cur = list->next;
    int i;
    int size = get_list_length(list);

    for(i = 0; i < size; i++)
    {
        printf("[ list pos[%d] = %s (addr : %d) ] \n", i, cur->text, &cur->text);
        cur = cur->next;
    }
}

void del_all_node(llist_t *list)
{
    llist_t *tmp = NULL;
    llist_t *cur = list;

    while (cur != NULL)
    {
        tmp = cur->next;
        free(cur);
        cur = tmp;
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