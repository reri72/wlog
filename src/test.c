#include "writelog.h"

int main(int argc, char const *argv[])
{
    _init_wlog( LOG_DEBUG, (KBYTE * KBYTE * KBYTE));
    _create_log("/home/github/printlog/src/","writelog.log");
    
    wlog_info("hello~ \n");
    wlog_debug("hi ~ \n");
    wlog_err("bye~ \n");
    wlog_warn("bi \n");

    sleep(5);

    _destroy_wlog();
    return 0;
}
