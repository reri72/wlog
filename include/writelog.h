#ifndef _WWRITELOG_H_
#define _WWRITELOG_H_

//  https://github.com/reri72

/*  definition  */
#define MAX_SIZE    4096
#define KBYTE  1024

#define W_FALSE       0
#define W_TRUE        1

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
    char fullpath[1024];
    int lfile;
} _loginfo_t;

typedef struct logq
{
    int max;
    int num;
    char *text[4096];
} logq_t;

extern _logset loglevel;

/*  function    */
int init_wlog(_logset set, int max);
void *log_thread(void *arg);
int get_lque_size(const logq_t *que);
int fwrite_text();
int logfile_size_check();
int lotate_file();
int create_logfile(char *dir, char *name);

void destroy_wlog();
void terminate_wlog(logq_t *que);
void clear_lque(logq_t *que);

void _changellevel(_logset set);
void _insert(const char *level, const char *filename, const int line, const char *funcname, const char * args, ...);
void add_logtext(char* newtext);
void print_lque(const logq_t *que);

#endif