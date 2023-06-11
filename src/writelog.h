#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <time.h>

typedef enum logset
{
    LOG_DEBUG = 3,
    LOG_ERROR = 2,
    LOG_WARN = 1,
} _logset;

extern _logset loglevel;

#define w3log(args, ...) \
    do { \
        if(loglevel >= 3) { \
           _writelog(__FILE__, __LINE__, __FUNCTION__, args, ##__VA_ARGS__); \
        } \
    } while(0)

#define w2log(args, ...) \
    do { \
        if(loglevel >= 2) { \
            _writelog(__FILE__, __LINE__, __FUNCTION__, args, ##__VA_ARGS__); \
        } \
    } while(0) 

#define w1log(args, ...) \
    do { \
        if(loglevel >= 1) { \
            _writelog(__FILE__, __LINE__, __FUNCTION__, args, ##__VA_ARGS__); \
        } \
    } while(0) 

void _writelog(const char *filename, const int line, const char *funcname, const char * args, ...);
