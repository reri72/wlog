#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <time.h>

typedef enum logset
{
    LOG_INFO = 0,
    LOG_WARN ,
    LOG_ERROR ,
    LOG_DEBUG
} _logset;

extern _logset loglevel;

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
        if(loglevel >= 1) { \
            _writelog("INFO", __FILE__, __LINE__, __FUNCTION__, args, ##__VA_ARGS__); \
        } \
    } while(0) 


bool createlogfile(char *dir);

void init_wlog();
void _getnow(char *buf);
void _writelog(const char *level, const char *filename, const int line, const char *funcname, const char * args, ...);
