#include "writelog.h"

int main(int argc, char const *argv[])
{
    init_wlog( LOG_DEBUG, (KBYTE * KBYTE * KBYTE));
    create_logfile("/home/github/printlog/src/","writelog.log");
    
    int i = 0;
    while(i < 30)
    {
        wlog_info("hello~ \n");
        wlog_debug("hi ~ \n");
        wlog_err("bye~ \n");
        wlog_warn("bi \n\n");
        n_sleep(0,500 );
        i++;
    }

    n_sleep(5,0);

    destroy_wlog();
    return 0;
}
