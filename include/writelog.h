#ifndef _WWRITELOG_H_
#define _WWRITELOG_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
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
           _insert("DEBUG", __FILE__, __LINE__, __FUNCTION__, args, ##__VA_ARGS__); \
        } \
    } while(0)

#define wlog_err(args, ...) \
    do { \
        if(loglevel >= 2) { \
            _insert("ERROR", __FILE__, __LINE__, __FUNCTION__, args, ##__VA_ARGS__); \
        } \
    } while(0) 

#define wlog_warn(args, ...) \
    do { \
        if(loglevel >= 1) { \
            _insert("WARN", __FILE__, __LINE__, __FUNCTION__, args, ##__VA_ARGS__); \
        } \
    } while(0) 

#define wlog_info(args, ...) \
    do { \
        if(loglevel >= 0) { \
            _insert("INFO", __FILE__, __LINE__, __FUNCTION__, args, ##__VA_ARGS__); \
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

typedef struct logq
{
    int max;
    int num;
    char *text[1024];
} logq_t;

logq_t logqueue;

_loginfo_t li;
extern _logset loglevel;

pthread_mutex_t mutex;
pthread_t tid;
volatile bool status;


/*  function    */
void *log_thread(void *arg);

int init_wlog(_logset set, int max);
void terminate_wlog(logq_t *que);
void destroy_wlog();
void _changellevel(_logset set);
void _insert(const char *level, const char *filename, const int line, const char *funcname, const char * args, ...);

bool create_logfile(char *dir, char *name);
bool lotate_file();

int fwrite_text();
int logfile_size_check();

int get_lque_size(const logq_t *que);
void print_lque(const logq_t *que);
void add_logtext(char* newtext);
void clear_lque(logq_t *que);

#endif