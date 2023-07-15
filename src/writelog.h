#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <pthread.h>

//  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  https://github.com/reri72/
//  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


/*  definition  */
#define MAX_SIZE    4096
#define MAX_ASIZE   2048
#define KBYTE  1024

#define FALSE       0;
#define TRUE        1;

#define wlog_debug(args, ...) \
    do { \
        if(loglevel >= 3) { \
           _insertlog("DEBUG", __FILE__, __LINE__, __FUNCTION__, args, ##__VA_ARGS__); \
        } \
    } while(0)

#define wlog_err(args, ...) \
    do { \
        if(loglevel >= 2) { \
            _insertlog("ERROR", __FILE__, __LINE__, __FUNCTION__, args, ##__VA_ARGS__); \
        } \
    } while(0) 

#define wlog_warn(args, ...) \
    do { \
        if(loglevel >= 1) { \
            _insertlog("WARN", __FILE__, __LINE__, __FUNCTION__, args, ##__VA_ARGS__); \
        } \
    } while(0) 

#define wlog_info(args, ...) \
    do { \
        if(loglevel >= 0) { \
            _insertlog("INFO", __FILE__, __LINE__, __FUNCTION__, args, ##__VA_ARGS__); \
        } \
    } while(0) 



/*  variables    */
typedef enum logset
{
    LOG_INFO = 0,
    LOG_WARN ,
    LOG_ERROR ,
    LOG_DEBUG
} _logset;

typedef struct loginfo
{
    char dir[512];
    char fname[128];
    char fullpath[641];
    FILE *lfile;
} _loginfo_t;

typedef struct llist
{
    struct llist *next;
    char* text;
} llist_t;

llist_t *loglist;

_loginfo_t li;
extern _logset loglevel;

pthread_mutex_t mutex;
pthread_t tid;
volatile bool status;



/*  function    */
void *log_thread(void *arg);

void _init_wlog(_logset set);
void _destroy_wlog();
void _changellevel(_logset set);
void _insertlog(const char *level, const char *filename, const int line, const char *funcname, const char * args, ...);
void _getnow(char *buf);

bool _create_log(char *dir, char *name);
bool _lotate_file();

int _writetext(llist_t *list);
int _szchk();

int get_list_length(llist_t *list);
void print_list(llist_t *list);
void add_list_item(llist_t *list, char* newtext);
void del_all_node(llist_t *list);

void n_sleep(int sec, int nsec);