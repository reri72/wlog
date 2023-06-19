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


//  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  https://github.com/reri72/
//  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


/*  definition  */
#define MAX_SIZE    4096
#define A_MAX_SIZE

#define wlog_debug(args, ...) \
    do { \
        if(loglevel >= 3) { \
           _writelog("DEBUG", __FILE__, __LINE__, __FUNCTION__, args, ##__VA_ARGS__); \
        } \
    } while(0)

#define wlog_err(args, ...) \
    do { \
        if(loglevel >= 2) { \
            _writelog("ERROR", __FILE__, __LINE__, __FUNCTION__, args, ##__VA_ARGS__); \
        } \
    } while(0) 

#define wlog_warn(args, ...) \
    do { \
        if(loglevel >= 1) { \
            _writelog("WARN", __FILE__, __LINE__, __FUNCTION__, args, ##__VA_ARGS__); \
        } \
    } while(0) 

#define wlog_info(args, ...) \
    do { \
        if(loglevel >= 0) { \
            _writelog("INFO", __FILE__, __LINE__, __FUNCTION__, args, ##__VA_ARGS__); \
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
    char dir[1024];
    char fname[512];
} _loginfo_t;

_loginfo_t li;
extern _logset loglevel;



/*  function    */
void init_wlog(_logset set);
void _writelog(const char *level, const char *filename, const int line, const char *funcname, const char * args, ...);
void _getnow(char *buf);

bool createlogfile(char *dir, char *name);