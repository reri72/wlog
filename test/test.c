#include "writelog.h"

int main(int argc, char const *argv[])
{
    init_wlog( LOG_DEBUG, (KBYTE * KBYTE * KBYTE));
    create_logfile("~/logdir/","testlog.log");
    
    int i = 0;

    while(i < 10)
    {
        wlog_info("hello~ \n");
        wlog_debug("hi ~ \n");
        wlog_err("bye~ \n");
        wlog_warn("bi \n\n");
        sleep(1);
        i++;
    }

    sleep(1);

    destroy_wlog();
    return 0;
}
